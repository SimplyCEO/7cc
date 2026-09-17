#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "l_append.h"
#include "l_remove.h"
#include "l_field.h"
#include "l_xml.h"

#include "main.h"
#include "safe_alloc.h"
#include "toolbox.h"

static void
l_pushcfunction(lua_State* L, int (*signal)(lua_State*), const char* name)
{
  lua_pushstring(L, name);
  lua_pushcfunction(L, signal);
  lua_settable(L, -3);
}

static int
l_api_doinclude(lua_State* L)
{
  int argc = lua_gettop(L);

  if (argc != 1)
  { return luaL_error(L, "usage: doinclude(str: \"lua/file.lua\")"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "doinclude(): Not a path."); }

  int i = 0;
  const char* buffer = lua_tostring(L, 1);
  const char* path = NULL;

  /* Open first given path. Open directories using `-I` option if not. */
  FILE* stream = fopen(buffer, "r");
  if (stream == NULL)
  {
    if (include != NULL)
    {
      for (; include[i]!=NULL; ++i)
      {
        path = strfmt("%s/%s", include[i], buffer);
        stream = fopen(path, "r");
        if (stream != NULL)
        { break; }
      }
    }
  }

  if (stream != NULL)
  { fclose(stream); }

  if (path == NULL)
  { return luaL_error(L, "doinclude(): Not a valid path."); }

  luaL_dofile(L, path);

  return 1;
}

static void
l_api_functions(lua_State* L)
{
  lua_newtable(L);
  lua_setglobal(L, "cc");
  lua_getglobal(L, "cc");

  l_pushcfunction(L, l_api_field_init, "field_init");
  l_pushcfunction(L, l_api_field_add,  "field_add");
  l_pushcfunction(L, l_api_append,     "append");
  l_pushcfunction(L, l_api_remove,     "remove");
  l_pushcfunction(L, l_api_xml_open,    "xml_open");
  l_pushcfunction(L, l_api_xml_get,     "xml_get");
  l_pushcfunction(L, l_api_xml_close,   "xml_close");

  lua_pop(L, 1);
}

const char*
l_getvalue(lua_State* L, const int index)
{
  const char* value = NULL;

  if      ( lua_isnumber(L, index) == true) { value = strfmt("%s", lua_tostring(L,  index)); }
  else if ( lua_isstring(L, index) == true) { value = strfmt("%s", lua_tostring(L,  index)); }
  else if (lua_isinteger(L, index) == true) { value = strfmt("%d", lua_tointeger(L, index)); }
  else if (lua_isboolean(L, index) == true) { value = (lua_toboolean(L, index) == 0) ? "false" : "true"; }
  else if (  lua_istable(L, index) == true) { value = "table"; }
  else { value = "nil"; }

  return value;
}

lua_State*
l_init(void)
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  lua_pushcfunction(L, l_api_doinclude);
  lua_setglobal(L, "doinclude");

  l_api_functions(L);

  return L;
}

int
l_run(lua_State* L, const char* filepath)
{
  if (luaL_dofile(L, filepath) != LUA_OK)
  {
    errprintf(lua_tostring(L, -1));
    return 1;
  }

  return 0;
}

int
l_free(lua_State* L)
{
  lua_close(L);

  return 0;
}

