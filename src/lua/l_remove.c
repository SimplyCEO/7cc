#include <stdio.h>

#include "l_remove.h"
#include "l_xml.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"

int
l_api_remove(lua_State* L)
{
  if (l_xml == NULL)
  { return luaL_error(L, "ERROR: No XML open in memory."); }

  if (lua_gettop(L) != 1)
  { return luaL_error(L, "usage: cc.remove(str: xpath)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "cc.remove(): Not a valid string."); }

  const char* section = lua_tostring(L, 1);

  XMLSize xml_index = xml_open(NULL);
  XMLObject* remove = xml_get(xml_index);
  XMLKey** remove_key = xml_key_init(1);

  remove_key = xml_key_add(remove_key, "xpath", strfmt("/%s", section));
  remove = xml_field_add(remove, "remove", remove_key);

  l_xml = xml_write(l_xml_index, remove->xml);

  remove_key = xml_key_free(remove_key);
  remove = xml_object_free(remove);

  return 1;
}

