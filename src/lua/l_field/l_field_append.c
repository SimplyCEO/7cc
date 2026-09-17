#include "l_field_append.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

int
l_api_field_append(lua_State* L)
{
  if (lua_gettop(L) != 2)
  { return luaL_error(L, "usage: cc.field.append(str: xpath, int: xml_index)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "cc.field.append(): First argument is not a valid string."); }

  if (lua_isinteger(L, 2) == false)
  { return luaL_error(L, "cc.field.append(): Second argument is not a valid XML index."); }

  const char* section = lua_tostring(L, 1);
  const XMLSize index = lua_tointeger(L, 2);

  XMLSize append_index = xml_open(NULL);
  XMLObject* append = xml_get(append_index);
  XMLKey** append_key = xml_key_init(1);

  append_key = xml_key_add(append_key, "xpath", strfmt("/%s", section));
  append = xml_field_add(append, "append", append_key);

  xml_write(append_index, xml_get(index)->xml);

  append_key = xml_key_free(append_key);

  lua_pushinteger(L, append_index);

  return 1;
}

