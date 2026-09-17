#include <stdio.h>

#include "l_append.h"
#include "l_xml.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"

int
l_api_append(lua_State* L)
{
  if (l_xml == NULL)
  { return luaL_error(L, "ERROR: No XML open in memory."); }

  if (lua_gettop(L) != 2)
  { return luaL_error(L, "usage: cc.append(str: xpath, str: xml_field)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "cc.append(): First argument is not a valid string."); }

  if (lua_isstring(L, 2) == false)
  { return luaL_error(L, "cc.append(): Second argument is not a valid XML field."); }

  const char* section = lua_tostring(L, 1);
  const char* field = lua_tostring(L, 2);

  XMLSize append_index = xml_open(NULL);
  XMLObject* append = xml_get(append_index);
  XMLKey** append_key = xml_key_init(1);

  append_key = xml_key_add(append_key, "xpath", strfmt("/%s", section));
  append = xml_field_add(append, "append", append_key);

  append = xml_write(append_index, field);
  l_xml = xml_write(l_xml_index, append->xml);

  append_key = xml_key_free(append_key);
  append = xml_object_free(append);

  return 1;
}

