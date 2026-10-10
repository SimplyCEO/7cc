#include <stdio.h>
#include <string.h>

#include "l_xml.h"

#include "xml.h"

#include "main.h"
#include "safe_alloc.h"
#include "toolbox.h"

static int
l_api_xml_open(lua_State* L)
{
  XMLSize xml_buffer_index = xml_open();

  lua_pushinteger(L, xml_buffer_index);

  return 1;
}

static int
l_api_xml_get(lua_State* L)
{
  if (lua_gettop(L) != 1)
  { return luaL_error(L, "\n" "usage: cc.xml.get(int: xml_index)"); }

  if (lua_isinteger(L, 1) == false)
  { return luaL_error(L, "\n" "cc.xml.get(): XML index not given."); }

  if (lua_tointeger(L, 1) < 0)
  { return 1; }

  XMLObject* object = xml_get(lua_tointeger(L, 1));

  lua_pushstring(L, object->data->content);

  return 1;
}

static int
l_api_xml_set(lua_State* L)
{
  if (lua_gettop(L) != 2)
  { return luaL_error(L, "\n" "usage: cc.xml.set(int: xml_index, str: xml_raw)"); }

  if (lua_isinteger(L, 1) == false)
  { return luaL_error(L, "\n" "cc.xml.set(): XML index not given."); }

  if (lua_isstring(L, 2) == false)
  { return luaL_error(L, "\n" "cc.xml.set(): Raw XML not given."); }

  if (lua_tointeger(L, 1) < 0)
  { return 1; }

  xml_write(lua_tointeger(L, 1), lua_tostring(L, 2));

  return 1;
}

static int
l_api_xml_close(lua_State* L)
{
  if (lua_gettop(L) != 1)
  { return luaL_error(L, "\n" "usage: cc.xml.close(int: xml_index)"); }

  if (lua_isinteger(L, 1) == false)
  { return luaL_error(L, "\n" "cc.xml.close(): XML index not given."); }

  if (lua_tointeger(L, 1) < 0)
  { return 1; }

  const int index = lua_tointeger(L, 1);

  if (index == l_xml_index)
  { return l_api_exit(L); }

  xml_close(index);

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
  l_pushcfunction(L, l_api_xml_close, "close");

  lua_pop(L, 1);

  return section;
}

