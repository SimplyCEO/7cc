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

static XMLKey**
_generate_key(lua_State* L, const int index)
{
  char* name = NULL;
  char* value = NULL;
  const char* token = NULL;
  XMLKey** xml_key = xml_key_init(1);

  if (lua_istable(L, index) == true)
  {
    lua_pushnil(L);
    while (lua_next(L, index) != 0)
    {
      int sub_index = lua_gettop(L);

      /* TODO: FIX SUBFIELD ARRAY.
      if (lua_type(L, sub_index-1) == LUA_TNUMBER)
      { lua_pop(L, 1); break; }
      */

      token = l_getvalue(L, sub_index-1);
      if (token != NULL)
      { name = strdup(token); }

      token = l_getvalue(L, sub_index);
      if (token != NULL)
      { value = strdup(token); }

      /* Ignore NULL values even if key exists. */
      if ((value == NULL) || (lua_istable(L, sub_index) == true)  )
      {
        lua_pop(L, 1);
        continue;
      }

      xml_key = xml_key_add(xml_key, name, value);

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
  char* name = NULL;
  char* value = NULL;
  const char* token = NULL;
  XMLObject* xml_field = xml_init(NULL);
  XMLKey** xml_key = _generate_key(L, index);

  /* Generate field with keys. */
  xml_key = xml_key_reorder(xml_key, XMLKEY_DEFAULT_ORDER);
  xml_field = xml_field_add(xml_field, field, (const XMLKey**)xml_key);
  xml_key = xml_key_free(xml_key);

  /* Generate sub field if found in table. */
  if (lua_istable(L, index) == true)
  {
    lua_pushnil(L);
    while (lua_next(L, index) != 0)
    {
      int sub_index = lua_gettop(L);

      /* TODO: FIX SUBFIELD ARRAY.
      if (lua_type(L, sub_index-1) == LUA_TNUMBER)
      {
        if (lua_istable(L, sub_index) == true)
        {
          lua_pushnil(L);
          while (lua_next(L, sub_index) != 0)
          {
            int nested_index = lua_gettop(L);

            token = l_getvalue(L, nested_index);
            if (token != NULL)
            { value = strdup(token); }

            XMLObject* xml_sub_field = xml_init(NULL);
            XMLKey** xml_sub_keys = _generate_key(L, lua_gettop(L));

            xml_sub_keys = xml_key_reorder(xml_sub_keys, XMLKEY_DEFAULT_ORDER);
            xml_sub_field = xml_field_add(xml_sub_field, buffer, (const XMLKey**)xml_sub_keys);
            xml_sub_keys = xml_key_free(xml_sub_keys);

            xml_field->xml = strins(xml_field->xml, xml_field->cursor, xml_sub_field->xml);
            xml_sub_field = xml_free(xml_sub_field);

            lua_pop(L, 1);
          }
        }
        printf("%s\n", xml_field->xml);
        lua_pop(L, 1);
        continue;
      }
      */

      token = l_getvalue(L, sub_index-1);
      if (token != NULL)
      { name = strdup(token); }

      token = l_getvalue(L, sub_index);
      if (token != NULL)
      { value = strdup(token); }

      /* Ignore NULL values even if key exists. */
      if (value == NULL)
      {
        lua_pop(L, 1);
        continue;
      }

      if (lua_istable(L, sub_index) == true)
      {
        buffer = name;
        XMLObject* xml_sub_field = _generate_field(L, sub_index, name);
        xml_field->xml = strins(xml_field->xml, xml_field->cursor, xml_sub_field->xml);
        xml_sub_field = xml_free(xml_sub_field);
        buffer = NULL;

        lua_pop(L, 1);
        continue;
      }

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
  { return luaL_error(L, "usage: field_add(\"field_name\", keys)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "field_add(): First argument is not a valid string."); }

  if (lua_istable(L, 2) == false)
  { return luaL_error(L, "field_add(): Second argument is not a valid table."); }

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
  { return luaL_error(L, "usage: field_add(keys)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "field_add(): Second argument is not a valid XML field."); }

  l_xml->xml = strins(l_xml->xml, l_xml->cursor, lua_tostring(L, 1));

  return 1;
}

