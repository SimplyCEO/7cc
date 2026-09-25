#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "l_field.h"
#include "l_xml.h"

#include "xml.h"
#include "xml_field.h"

#include "main.h"
#include "safe_alloc.h"
#include "toolbox.h"

XMLSize    l_xml_index = 0;
XMLObject* l_xml = NULL;

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
  if ((include != NULL) && (iffile(buffer) == true))
  {
    FILE* stream = fopen(buffer, "r");

    for (; include[i]!=NULL; ++i)
    {
      path = strfmt("%s/%s", include[i], buffer);
      if (iffile(path) == true)
      { break; }
    }

    fclose(stream);
  }

  if (path == NULL)
  { return luaL_error(L, "doinclude(): Not a valid path."); }

  luaL_dofile(L, path);

  return 1;
}

static int
l_api_get_architecture(lua_State* L)
{
#if (BUILD64 == 0)
  lua_pushstring(L, "32");
#else
  lua_pushstring(L, "64");
#endif

  return 1;
}

void
l_pushcfunction(lua_State* L, int (*signal)(lua_State*), const char* name)
{
  lua_pushstring(L, name);
  lua_pushcfunction(L, signal);
  lua_settable(L, -3);
}

void
l_pushtable(lua_State* L, const int index, const char* reference)
{
  lua_getglobal(L, reference);
  lua_setfield(L, index, reference);
}

static void
l_api_functions(lua_State* L)
{
  lua_newtable(L);
  lua_setglobal(L, "cc");
  lua_getglobal(L, "cc");

  const int index = lua_gettop(L);

  l_pushcfunction(L, l_api_get_architecture, "get_architecture");
  l_pushtable(L, index, l_api_field(L));
  l_pushtable(L, index, l_api_xml(L));

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

  l_xml_index = xml_open(output);
  l_xml = xml_get(l_xml_index);

  /* XML file name field. */
  if (output != NULL)
  {
    XMLSize    field_index = xml_open(NULL);
    XMLObject* field = xml_get(field_index);
    char* buffer = strdup(basename(output));
    buffer = strcut(buffer, 0, strlen(buffer) - 5);
    field = xml_field_add(field, buffer, NULL);
    l_xml = xml_write(l_xml_index, field->xml);
    l_xml->cursor += field->cursor;
    buffer = safe_free(buffer);
    field = xml_object_free(field);
  }

  return L;
}

int
l_run(lua_State* L, const char* filepath)
{
  if (luaL_dofile(L, filepath) != LUA_OK)
  {
    error(lua_tostring(L, -1));
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

