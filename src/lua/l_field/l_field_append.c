#include "l_field_append.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"
#include "xml_object.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

int
l_api_field_append(lua_State* L)
{
  if (lua_gettop(L) != 2)
  {
    return luaL_error(L, "\n"
      "usage: cc.field.append(str: xpath, int: xml_index)\n"
      "   or: cc.field.append(str: xpath, str: xml_field)");
  }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "\n" "cc.field.append(): First argument is not a valid string."); }

  bool isinteger = true;
  if (lua_isinteger(L, 2) == false)
  {
    if (lua_isstring(L, 2) == false)
    { return luaL_error(L, "\n" "cc.field.append(): Second argument is neither a valid XML index nor a XML field."); }
    isinteger = false;
  }

  const char* section = lua_tostring(L, 1);
  XMLObject* buffer = NULL;

  if (isinteger == true)
  { buffer = xml_get(lua_tointeger(L, 2)); }
  else
  {
    buffer = xml_object_alloc();
    buffer->data->content = strdup(lua_tostring(L, 2));
  }

  XMLSize object_index = xml_open();
  XMLObject* object = xml_get(object_index);
  object->data->keys = xml_key_init(1);

  object->data->keys = xml_key_add(object->data->keys, "xpath", strfmt("/%s", section));
  object = xml_field_init(object, "append", object->data->keys);
  object = xml_field_add(object, buffer);

  lua_pushinteger(L, object_index);

  return 1;
}

