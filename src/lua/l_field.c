#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "l_xml.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

const char* buffer = NULL;
XMLObject* xml_buffer = NULL;

static char*
_strbuff(lua_State* L, const int index)
{
  static char output[64] = {0};
  memset(output, '\0', 64);

  strncpy(output, l_getvalue(L, index), 64);

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
      if (lua_type(L, sub_index - 1) == LUA_TNUMBER) { lua_pop(L, 2); break;    }
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

static XMLObject*
_generate_field(lua_State* L, const int index, const char* field)
{
  int i = 0;
  int table_length = 0;
  char* name = NULL;
  char* value = NULL;
  XMLObject* xml_field = xml_init(NULL);
  XMLKey** xml_key = _generate_key(L, index);

  if (buffer == NULL)
  { buffer = field; }

  /* Generate field with keys. */
  xml_key = xml_key_reorder(xml_key, XMLKEY_DEFAULT_ORDER);
  xml_field = xml_field_add(xml_field, field, xml_key);
  xml_key = xml_key_free(xml_key);

  /* Generate sub field if found in table. */
  if (lua_istable(L, index) == true)
  {
    lua_len(L, index);
    bool is_table_array = ((table_length = lua_tointeger(L, -1)) == 0) ? false : true;
    lua_pop(L, 1);

    if (is_table_array == false)
    {
      /* lua_next() can not receive numeric keys. */
      lua_pushnil(L);
      while (lua_next(L, index) != 0)
      {
        int sub_index = lua_gettop(L);

        if (strncmp((value = _strdupfree(value, _strbuff(L, sub_index))), "table", 3) == 0)
        {
          XMLObject* xml_sub_field = _generate_field(L, sub_index, (buffer = name = _strdupfree(name, _strbuff(L, sub_index - 1))));
          xml_field->xml = strins(xml_field->xml, xml_field->cursor, xml_sub_field->xml);
          xml_sub_field = xml_free(xml_sub_field);
          buffer = NULL;
        }

        lua_pop(L, 1);
      }
    }

    /* Iterate table array. */
    for (i=table_length; i>=1; --i)
    {
      lua_rawgeti(L, index, i);

      int sub_index = lua_gettop(L);

      XMLObject* xml_sub_field = xml_init(NULL);
      xml_key = _generate_key(L, sub_index);

      xml_key = xml_key_reorder(xml_key, XMLKEY_DEFAULT_ORDER);
      xml_sub_field = xml_field_add(xml_sub_field, buffer, xml_key);
      xml_key = xml_key_free(xml_key);

      xml_field->xml = strins(xml_field->xml, xml_field->cursor, xml_sub_field->xml);
      xml_sub_field = xml_free(xml_sub_field);

      lua_pop(L, 1);
    }
  }

  name = safe_free(name);
  value = safe_free(value);

  return xml_field;
}

int
l_api_field_init(lua_State* L)
{
  if (lua_gettop(L) < 2)
  { return luaL_error(L, "usage: cc.field_init(str: \"field_name\", table: keys)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "cc.field_init(): First argument is not a valid string."); }

  if (lua_istable(L, 2) == false)
  { return luaL_error(L, "cc.field_init(): Second argument is not a valid table."); }

  const char* field = lua_tostring(L, 1);
  XMLObject* xml_field = _generate_field(L, 2, field);

  lua_pushstring(L, xml_field->xml);

  xml_field = xml_free(xml_field);

  return 1;
}

int
l_api_field_add(lua_State* L)
{
  if (l_xml == NULL)
  { return luaL_error(L, "ERROR: No XML open in memory."); }

  if (lua_gettop(L) < 1)
  { return luaL_error(L, "usage: cc.field_add(str: xml_field)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "cc.field_add(): Not a valid XML field."); }

  l_xml->xml = strins(l_xml->xml, l_xml->cursor, lua_tostring(L, 1));

  return 1;
}

