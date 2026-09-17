#include <stdio.h>
#include <string.h>

#include "l_xml.h"

#include "xml.h"
#include "xml_field.h"
#include "xml_parse.h"

#include "main.h"
#include "safe_alloc.h"
#include "toolbox.h"

XMLSize    l_xml_index = 0;
XMLObject* l_xml = NULL;

static int
l_api_xml_open(lua_State* L)
{
  if (l_xml == NULL)
  {
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

    lua_pushinteger(L, l_xml_index);

    return 1;
  }

  XMLSize xml_buffer_index = xml_open(NULL);

  xml_write(xml_buffer_index, "");
  lua_pushinteger(L, xml_buffer_index);

  return 1;
}

static int
l_api_xml_get(lua_State* L)
{
  if (lua_gettop(L) != 1)
  { return luaL_error(L, "usage: cc.xml.get(int: xml_index)"); }

  if (lua_isinteger(L, 1) == false)
  { return luaL_error(L, "cc.xml.get(): XML index not given."); }

  lua_pushstring(L, xml_get(lua_tointeger(L, 1))->xml);

  return 1;
}

static int
l_api_xml_set(lua_State* L)
{
  if (lua_gettop(L) != 2)
  { return luaL_error(L, "usage: cc.xml.set(int: xml_index, str: xml_raw)"); }

  if (lua_isinteger(L, 1) == false)
  { return luaL_error(L, "cc.xml.set(): XML index not given."); }

  if (lua_isstring(L, 2) == false)
  { return luaL_error(L, "cc.xml.set(): Raw XML not given."); }

  xml_set(lua_tointeger(L, 1), lua_tostring(L, 2));

  return 1;
}

static int
l_api_xml_write(lua_State* L)
{
  if (lua_gettop(L) != 2)
  { return luaL_error(L, "usage: cc.xml.write(int: xml_index, str: xml_raw)"); }

  if (lua_isinteger(L, 1) == false)
  { return luaL_error(L, "cc.xml.write(): XML index not given."); }

  if (lua_isstring(L, 2) == false)
  { return luaL_error(L, "cc.xml.write(): Raw XML not given."); }

  xml_write(lua_tointeger(L, 1), lua_tostring(L, 2));

  return 1;
}

static int
l_api_xml_close(lua_State* L)
{
  if (l_xml == NULL)
  { return luaL_error(L, "ERROR: No XML open in memory."); }

  l_xml = xml_parse_fix_structure(l_xml);
  l_xml = xml_parse_identation(l_xml);

  switch (identation)
  {
    case -1: l_xml = xml_parse_translate(l_xml, "", ""); break;
    case 1: l_xml = xml_parse_translate(l_xml, "\n", "  "); break;
    default: l_xml = xml_parse_translate(l_xml, "\n", "\t"); break;
  }

  /* View XML file instead of compiling object. */
  if (compile == false)
  {
    lua_pushstring(L, l_xml->xml);
    lua_setglobal(L, "cc_output");
    xml_close(-1);
    return 1;
  }

  FILE* stream = fopen(l_xml->path, "w");
  size_t i = 0;
  for (; i<strlen(l_xml->xml); ++i)
  { fputc(l_xml->xml[i], stream); }
  fclose(stream);

  xml_close(-1);

  return 1;
}

const char*
l_api_xml(lua_State* L)
{
  const char* section = "xml";

  lua_newtable(L);
  lua_setglobal(L, section);
  lua_getglobal(L, section);

  l_pushcfunction(L, l_api_xml_open,  "open");
  l_pushcfunction(L, l_api_xml_get,   "get");
  l_pushcfunction(L, l_api_xml_set,   "set");
  l_pushcfunction(L, l_api_xml_write, "write");
  l_pushcfunction(L, l_api_xml_close, "close");

  lua_pop(L, 1);

  return section;
}

