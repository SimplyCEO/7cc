#include <stdio.h>

#include "lua.h"
#include "l_xml.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"

#include "types.h"

int
l_api_field_add(lua_State* L)
{
  int argc = lua_gettop(L);
  if (argc < 2)
  { return luaL_error(L, "usage: field_add(\"field_name\", keys)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "field_add(): First argument is not a valid string."); }

  if (lua_istable(L, 2) == false)
  { return luaL_error(L, "field_add(): Second argument is not a valid table."); }

  XMLObject* xml_field = xml_init(NULL);
  const char* field = lua_tostring(L, 1);
  XMLKey** xml_key = xml_key_init(1);

  lua_pushnil(L);
  while (lua_next(L, 2))
  {
    const char* name = lua_tostring(L, -2);
    const char* value = l_getvalue(L, -1);

    xml_key = xml_key_add(xml_key, name, value);

    lua_pop(L, 1);
  }

  xml_field = xml_field_add(xml_field, field, (const XMLKey**)xml_key);

  lua_pushstring(L, xml_field->xml);

  xml_key = xml_key_free(xml_key);
  xml_field = xml_free(xml_field);

  return 1;
}

