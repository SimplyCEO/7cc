#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "l_field.h"
#include "l_xml.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_parse.h"

#include "main.h"
#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

bool       api_exit    = false;
XMLSize    l_xml_index = 0;
XMLObject* head_info   = NULL;
XMLObject* l_xml       = NULL;

static XMLObject*
l_api_xml_assemble(XMLObject* object)
{
  object = xml_parse_assemble(object);

  switch (identation)
  {
    case -1: object = xml_parse_translate(object, "",   ""  ); break;
    case 1:  object = xml_parse_translate(object, "  ", "\n"); break;
    default: object = xml_parse_translate(object, "\t", "\n"); break;
  }

  object->print = true;

  return object;
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
  if (include != NULL)
  {
    for (; include[i]!=NULL; ++i)
    {
      path = strfmt("%s/%s", include[i], buffer);
      if (iffile(path) == false) { path = NULL; continue; }
      break;
    }
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

int
l_api_exit(lua_State* L)
{
  api_exit = true;

  l_xml = l_api_xml_assemble(l_xml);

  /* View XML file instead of compiling object. */
  if (compile == false)
  {
    xml_close(0);

    luaL_error(L, "API exit signal.");
    return 0;
  }

  FILE* stream = fopen(l_xml->info->path, "w");
  size_t i = 0;
  for (; i<strlen(l_xml->data->content); ++i)
  { fputc(l_xml->data->content[i], stream); }
  fclose(stream);

  head_info->print = false;
  l_xml->print = false;

  xml_close(0);

  luaL_error(L, "API exit signal.");
  return 0;
}

void
l_pushcfunction(lua_State* L, int (*signal)(lua_State*), const char* name)
{
  lua_pushstring(L, name);
  lua_pushcfunction(L, signal);
  lua_settable(L, -3);
}

void
l_pushstring(lua_State* L, const char* value, const char* name)
{
  lua_pushstring(L, name);
  lua_pushstring(L, value);
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
  l_pushcfunction(L, l_api_exit, "exit");
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

static void
l_api_output_xml(void)
{
  head_info = xml_object_alloc();
  head_info = xml_object_version(head_info);
  head_info = l_api_xml_assemble(head_info);
  xml_add(head_info);

  l_xml_index = xml_open();
  l_xml = xml_get(l_xml_index);

  if (output == NULL) { output = "example"; }

  /* XML file name field. */
  char*       buffer   = strdup(output);
  const char* dir_path = dirname(output);

  /* Directory is either empty or a file name. */
  if ((dir_path[0] == '\0') || (strcmp(dir_path, output) == 0))
  {
    strcpy(buffer, basename(output));
  }

  l_xml->info->path = strdup(buffer);
  buffer = safe_free(buffer);

  buffer = strdup(basename(output));

  const char* filetype = strchr(buffer, '.');
  if (filetype != NULL)
  {
    buffer = strcut(buffer, 0, strlen(buffer) - strlen(filetype));
  }

  l_xml = xml_field_init(l_xml, buffer, l_xml->data->keys);

  output = NULL;
  buffer = safe_free(buffer);
}

lua_State*
l_init(void)
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  lua_pushcfunction(L, l_api_doinclude);
  lua_setglobal(L, "doinclude");

  l_api_functions(L);
  l_api_output_xml();

  return L;
}

int
l_run(lua_State* L, const char* filepath)
{
  if ((luaL_dofile(L, filepath) != LUA_OK) && (api_exit == false))
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

