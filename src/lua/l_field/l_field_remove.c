#include "l_field_remove.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

int
l_api_field_remove(lua_State* L)
{
  if (lua_gettop(L) != 2)
  {
    return luaL_error(L, "\n"
      "usage: cc.field.remove(str: xpath, int: xml_index)\n"
      "   or: cc.field.remove(str: xpath, str: xml_field)");
  }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "\n" "cc.field.remove(): First argument is not a valid string."); }

  bool isinteger = true;
  if (lua_isinteger(L, 2) == false)
  {
    if (lua_isstring(L, 2) == false)
    { return luaL_error(L, "\n" "cc.field.remove(): Second argument is neither a valid XML index nor a XML field."); }
    isinteger = false;
  }

  const char* section = lua_tostring(L, 1);
  const char* xml_buffer = NULL;

  if (isinteger == true)
  {
    const XMLSize index = lua_tointeger(L, 2);
    xml_buffer = (index != -1) ? xml_get(index)->xml : "";
  }
  else
  { xml_buffer = lua_tostring(L, 2); }

  XMLSize remove_index = xml_open(NULL);
  XMLObject* remove = xml_get(remove_index);
  XMLKey** remove_key = xml_key_init(1);

  remove_key = xml_key_add(remove_key, "xpath", strfmt("/%s", section));
  remove = xml_field_add(remove, "remove", remove_key);

  xml_write(remove_index, xml_buffer);

  remove_key = xml_key_free(remove_key);

  lua_pushinteger(L, remove_index);

  return 1;
}

