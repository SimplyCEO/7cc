#include <stdio.h>
#include <string.h>

#include "l_field.h"
#include "l_field_append.h"
#include "l_field_remove.h"
#include "l_generate.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_object.h"

#include "main.h"
#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

static int
l_api_field_create(lua_State* L)
{
  bool        close  = false;
  const char* field  = NULL;
  XMLSize     index  = 0;
  XMLObject*  object = NULL;

  if (lua_gettop(L) < 2)           { return luaL_error(L, "\n" "usage: cc.field.create(str: \"field_name\", table: keys, *bool: close)"); }
  if (lua_isstring(L, 1) == false) { return luaL_error(L, "\n" "cc.field.create(): First argument is not a valid string."); }
  if (lua_istable(L, 2) == false)  { return luaL_error(L, "\n" "cc.field.create(): Second argument is not a valid table."); }

  if (lua_isboolean(L, 3) == true)
  { close = lua_toboolean(L, 3); }

  field  = lua_tostring(L, 1);
  object = l_api_generate_field(L, 2, field);
  index  = xml_add(object);

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
    object = xml_object_free(object);
    return 1;
  }

  lua_pushinteger(L, index);

  return 1;
}

int
l_api_field_add(lua_State* L)
{
  bool       isinteger     = true;
  XMLSize    index         = 0;
  XMLObject* object        = NULL;
  XMLObject* object_buffer = NULL;

  if (lua_gettop(L) != 2)
  {
    return luaL_error(L, "\n"
      "usage: cc.field.add(str: xpath, int: xml_index)\n"
      "   or: cc.field.add(str: xpath, str: xml_field)");
  }

  if (lua_isinteger(L, 1) == false)  { return luaL_error(L, "\n" "cc.field.add(): XML destination index not given."); }
  if (lua_isinteger(L, 2) == false)
  {
    if (lua_isstring(L, 2) == false) { return luaL_error(L, "\n" "cc.field.add(): Second argument is neither a valid XML index nor a XML field."); }
    isinteger = false;
  }

  if (lua_tointeger(L, 2) < 0)
  { return 1; }

  index = lua_tointeger(L, 1);
  object = xml_get(index);

  if (isinteger == true)
  { object_buffer = xml_get(lua_tointeger(L, 2)); }
  else
  {
    object_buffer = xml_object_alloc();
    object_buffer->data->content = strdup(lua_tostring(L, 2));
    object_buffer->info->size = strlen(object_buffer->data->content);
  }

  if (object_buffer != NULL)
  { object = xml_field_add(object, object_buffer); }

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

