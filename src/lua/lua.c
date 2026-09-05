#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "l_append.h"
#include "l_xml.h"

#include "safe_alloc.h"
#include "toolbox.h"

lua_State* L = NULL;

static void
l_api_functions(void)
{
  lua_pushcfunction(L, c_api_append);
  lua_setglobal(L, "append");

  lua_pushcfunction(L, c_api_openxml);
  lua_setglobal(L, "openxml");

  lua_pushcfunction(L, c_api_closexml);
  lua_setglobal(L, "closexml");
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

