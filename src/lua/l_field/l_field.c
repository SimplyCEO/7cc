#include <stdio.h>
#include <string.h>

#include "l_field.h"
#include "l_field_append.h"
#include "l_field_remove.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"
#include "xml_object.h"

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
  XMLKey** keys = xml_key_init(1);

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
        { keys = xml_key_add(keys, (name = _strdupfree(name, _strbuff(L, sub_index - 1))), value); }
      }

      lua_pop(L, 1);
    }
  }

  name = safe_free(name);
  value = safe_free(value);

  return keys;
}

static XMLObject*
_generate_field(lua_State* L, const int index, const char* field)
{
  int i = 0;
  int table_length = 0;
  char* name = NULL;
  char* value = NULL;
  XMLObject* object = xml_object_alloc();

  object->data->keys = _generate_key(L, index);

#if (BUILD64 == 0)
  if (xml32_index == -32)
  {
    object = xml_object_garbage(object);
    return object;
  }
#endif

  if (buffer == NULL)
  { buffer = field; }

  /* Generate field with keys. */
  object->data->keys = xml_key_reorder(object->data->keys, XMLKEY_DEFAULT_ORDER);
  object = xml_field_init(object, field, object->data->keys);

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
        object = xml_object_garbage(object);
        object = xml_object_alloc();

        /* Iterate table array. */
        for (i=table_length; i>=1; --i)
        {
          lua_rawgeti(L, index, i);

          XMLObject* field_object = xml_object_alloc();
          field_object->data->keys = _generate_key(L, lua_gettop(L));

          field_object->data->keys = xml_key_reorder(field_object->data->keys, XMLKEY_DEFAULT_ORDER);
          field_object = xml_field_init(field_object, buffer, field_object->data->keys);

          object = xml_field_add(object, field_object);

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
          XMLObject* field_object = _generate_field(L, sub_index, name);

#if (BUILD64 == 0)
          if (field_object->garbage == true)
          {
            lua_pop(L, 2);
            break;
          }
#endif

          object = xml_field_add(object, field_object);
          buffer = NULL;
        }

        lua_pop(L, 1);
      }
    }
  }
  while (0);

  name = safe_free(name);
  value = safe_free(value);

  return object;
}

static int
l_api_field_create(lua_State* L)
{
  XMLSize xml_index = 0;
  bool close = false;
  const char* field = NULL;

  if (lua_gettop(L) < 2)
  { return luaL_error(L, "\n" "usage: cc.field.create(str: \"field_name\", table: keys, *bool: close)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "\n" "cc.field.create(): First argument is not a valid string."); }

  if (lua_istable(L, 2) == false)
  { return luaL_error(L, "\n" "cc.field.create(): Second argument is not a valid table."); }

  if (lua_isboolean(L, 3) == true)
  { close = lua_toboolean(L, 3); }

  field = lua_tostring(L, 1);
  XMLObject* object = _generate_field(L, 2, field);
  xml_index = xml_set(object, xml_index);

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

  if (close == true)
  {
    lua_pushstring(L, object->data->content);
    object = xml_object_garbage(object);
    return 1;
  }

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
  XMLObject* object = xml_get(dest);
  XMLObject* buffer = NULL;

  if (isinteger == true)
  { buffer = xml_get(lua_tointeger(L, 2)); }
  else
  {
    buffer = xml_object_alloc();
    buffer->data->content = strdup(lua_tostring(L, 2));
  }

  object = xml_field_add(object, buffer);

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

