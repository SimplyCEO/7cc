#include <string.h>

#include "l_field_remove.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"
#include "xml_object.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

int
l_api_field_remove(lua_State* L)
{
  bool        isinteger     = true;
  const char* section       = NULL;
  XMLSize     index         = 0;
  XMLObject*  object        = NULL;
  XMLObject*  object_buffer = NULL;

  if (lua_gettop(L) != 2)
  {
    return luaL_error(L, "\n"
      "usage: cc.field.remove(str: xpath, int: xml_index)\n"
      "   or: cc.field.remove(str: xpath, str: xml_field)");
  }

  if (lua_isstring(L, 1) == false)   { return luaL_error(L, "\n" "cc.field.remove(): First argument is not a valid string."); }
  if (lua_isinteger(L, 2) == false)
  {
    if (lua_isstring(L, 2) == false) { return luaL_error(L, "\n" "cc.field.remove(): Second argument is neither a valid XML index nor a XML field."); }
    isinteger = false;
  }

  if (lua_tointeger(L, 2) < 0)
  { return 1; }

  section = lua_tostring(L, 1);

  if (isinteger == true)
  { object_buffer = xml_get(lua_tointeger(L, 2)); }
  else
  {
    object_buffer = xml_object_alloc();
    object_buffer->data->content = strdup(lua_tostring(L, 2));
    object_buffer->info->size = strlen(object_buffer->data->content);
  }

  object = xml_object_alloc();
  object->data->keys = xml_key_init(1);
  object->data->keys = xml_key_add(object->data->keys, "xpath", strfmt("/%s", section));

  object = xml_field_init(object, "remove", object->data->keys);

  if (object_buffer != NULL)
  { object = xml_field_add(object, object_buffer); }

  index = xml_add(object);

  lua_pushinteger(L, index);

  return 1;
}

