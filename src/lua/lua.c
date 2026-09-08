#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "l_append.h"
#include "l_field.h"
#include "l_xml.h"

#include "safe_alloc.h"
#include "toolbox.h"

lua_State* L = NULL;

static void
l_api_functions(void)
{
  lua_pushcfunction(L, l_api_field_add);
  lua_setglobal(L, "field_add");

  lua_pushcfunction(L, l_api_append);
  lua_setglobal(L, "append");

  lua_pushcfunction(L, l_api_openxml);
  lua_setglobal(L, "openxml");

  lua_pushcfunction(L, l_api_closexml);
  lua_setglobal(L, "closexml");
}

const char*
l_getvalue(lua_State* L, const int index)
{
  const char* value = NULL;

  if      (lua_isstring(L,  index) == true) { value = strfmt("%s",   lua_tostring(L,  index)); }
  else if (lua_isinteger(L, index) == true) { value = strfmt("%d",   lua_tointeger(L, index)); }
  else if (lua_isnumber(L,  index) == true) { value = strfmt("%f.1", lua_tonumber(L,  index)); }
  else if (lua_isboolean(L, index) == true) { value = (lua_toboolean(L, index) == 0) ? "false" : "true"; }
  else { value = "nil"; }

  return value;
}

int
l_init(void)
{
  L = luaL_newstate();
  luaL_openlibs(L);

  l_api_functions();

  return 0;
}

int
l_run(const char* filepath)
{
  if (luaL_dofile(L, filepath) != LUA_OK)
  {
    errprintf(lua_tostring(L, -1));
    return 1;
  }

  return 0;
}

int
l_free(void)
{
  lua_close(L);

  return 0;
}

