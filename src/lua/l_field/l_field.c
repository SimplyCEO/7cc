#include <stdio.h>
#include <string.h>

#include "l_field.h"
#include "l_field_append.h"
#include "l_field_remove.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"

#include "main.h"
#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

#if (BUILD64 == 0)
XMLSize xml32_index = 0;
#endif
const char* buffer = NULL;

static char*
_strbuff(lua_State* L, const int index)
{
  static char output[64] = {0};
  memset(output, '\0', 64);

  strncpy(output, l_getvalue(L, index), 63);

  return output;
}

static char*
_strdupfree(char* src, const char* buffer)
{
  src = safe_free(src);

  src = strdup(buffer);

  return src;
}

static XMLKey**
_generate_key(lua_State* L, const int index)
{
  char* name = NULL;
  char* value = NULL;
  XMLKey** xml_key = xml_key_init(1);

  if (lua_istable(L, index) == true)
  {
    lua_pushnil(L);
    while (lua_next(L, index) != 0)
    {
      int sub_index = lua_gettop(L);

      /* Currently there is no support for array keys. */
      if (lua_type(L, sub_index - 1) == LUA_TNUMBER)
      {
#if (BUILD64 == 0)
        xml32_index = -32;
#endif
        lua_pop(L, 2);
        break;
      }
      if (lua_istable(L, sub_index) == false)
      {
        /* Ignore NULL values even if key exists. */
        if (strncmp((value = _strdupfree(value, _strbuff(L, sub_index))), "nil", 3) != 0)
        { xml_key = xml_key_add(xml_key, (name = _strdupfree(name, _strbuff(L, sub_index - 1))), value); }
      }

      lua_pop(L, 1);
    }
  }

  name = safe_free(name);
  value = safe_free(value);

  return xml_key;
}

static XMLSize
_generate_field(lua_State* L, const int index, const char* field)
{
  int i = 0;
  int table_length = 0;
  char* name = NULL;
  char* value = NULL;
  XMLSize xml_field_index = xml_open(NULL);
  XMLObject* xml_field = xml_get(xml_field_index);
  XMLKey** xml_key = _generate_key(L, index);

#if (BUILD64 == 0)
  if (xml32_index == -32)
  {
    xml_field = xml_object_free(xml_field);
    xml_key = xml_key_free(xml_key);
    return xml32_index;
  }
#endif

  if (buffer == NULL)
  { buffer = field; }

  /* Generate field with keys. */
  xml_key = xml_key_reorder(xml_key, XMLKEY_DEFAULT_ORDER);
  xml_field = xml_field_add(xml_field, field, xml_key);
  xml_key = xml_key_free(xml_key);

  do
  {
    /* Generate sub field if found in table. */
    if (lua_istable(L, index) == true)
    {
      lua_len(L, index);
      bool is_table_array = ((table_length = (int)lua_tointeger(L, -1)) != 0);
      lua_pop(L, 1);

      if (is_table_array == true)
      {
        xml_field = xml_object_free(xml_field);
        xml_field_index = xml_open(NULL);
        xml_field = xml_get(xml_field_index);

        /* Iterate table array. */
        for (i=table_length; i>=1; --i)
        {
          lua_rawgeti(L, index, i);

          int sub_index = lua_gettop(L);

          XMLObject* xml_sub_field = xml_get(xml_open(NULL));
          xml_key = _generate_key(L, sub_index);

          xml_key = xml_key_reorder(xml_key, XMLKEY_DEFAULT_ORDER);
          xml_sub_field = xml_field_add(xml_sub_field, buffer, xml_key);
          xml_key = xml_key_free(xml_key);

          xml_field = xml_write(xml_field_index, xml_sub_field->xml);
          xml_sub_field = xml_object_free(xml_sub_field);

          lua_pop(L, 1);
        }

        break;
      }

      /* lua_next() can not receive numeric keys. */
      lua_pushnil(L);
      while (lua_next(L, index) != 0)
      {
        int sub_index = lua_gettop(L);

        if (strncmp((value = _strdupfree(value, _strbuff(L, sub_index))), "table", 3) == 0)
        {
          buffer = name = _strdupfree(name, _strbuff(L, sub_index - 1));
          XMLSize xml_sub_field_index = _generate_field(L, sub_index, name);

#if (BUILD64 == 0)
          if ((xml32_index = xml_sub_field_index) == -32)
          {
            lua_pop(L, 2);
            break;
          }
#endif

          XMLObject* xml_sub_field = xml_get(xml_sub_field_index);
          xml_field = xml_write(xml_field_index, xml_sub_field->xml);
          xml_sub_field = xml_object_free(xml_sub_field);
          buffer = NULL;
        }

        lua_pop(L, 1);
      }
    }
  }
  while (0);

  name = safe_free(name);
  value = safe_free(value);

  return xml_field_index;
}

static int
l_api_field_create(lua_State* L)
{
  if (lua_gettop(L) != 2)
  { return luaL_error(L, "\n" "usage: cc.field.create(str: \"field_name\", table: keys)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "\n" "cc.field.create(): First argument is not a valid string."); }

  if (lua_istable(L, 2) == false)
  { return luaL_error(L, "\n" "cc.field.create(): Second argument is not a valid table."); }

  const char* field = lua_tostring(L, 1);
  XMLSize xml_index = _generate_field(L, 2, field);

#if (BUILD64 == 0)
  if (xml32_index == -32)
  {
    if (warning_p >= 2)
    {
      warning("\n"
        "cc.field.create(): lua_rawgeti() (32‑bit) may return arrays as nil.\n"
        "                   Generate each field separately to avoid this issue."
      );
    }
    xml32_index = 0;
  }
#endif

  lua_pushinteger(L, xml_index);

  return 1;
}

static int
l_api_field_add(lua_State* L)
{
  if (lua_gettop(L) != 2)
  {
    return luaL_error(L, "\n"
      "usage: cc.field.add(int: xml_dest_index, int: xml_src_index)\n"
      "   or: cc.field.add(int: xml_dest_index, str: xml_src_field)");
  }

  if (lua_isinteger(L, 1) == false)
  { return luaL_error(L, "\n" "cc.field.add(): XML destination index not given."); }

  bool isinteger = true;
  if (lua_isinteger(L, 2) == false)
  {
    if (lua_isstring(L, 2) == false)
    { return luaL_error(L, "\n" "cc.field.add(): Second argument is neither a valid XML index nor a XML field."); }
    isinteger = false;
  }

  const XMLSize dest = lua_tointeger(L, 1);
  const char* xml_buffer = NULL;

  if (isinteger == true)
  {
    const XMLSize src = lua_tointeger(L, 2);
    xml_buffer = (src != -1) ? xml_get(src)->xml : "";
  }
  else
  { xml_buffer = lua_tostring(L, 2); }

  xml_write(dest, xml_buffer);

  return 1;
}

const char*
l_api_field(lua_State* L)
{
  const char* section = "field";

  lua_newtable(L);
  lua_setglobal(L, section);
  lua_getglobal(L, section);

  l_pushcfunction(L, l_api_field_create, "create");
  l_pushcfunction(L, l_api_field_add,    "add");
  l_pushcfunction(L, l_api_field_append, "append");
  l_pushcfunction(L, l_api_field_remove, "remove");

  lua_pop(L, 1);

  return section;
}

