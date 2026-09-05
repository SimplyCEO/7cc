#include <stdio.h>
#include <string.h>

#include "l_xml.h"

#include "xml.h"

#include "main.h"

XMLObject* l_xml = NULL;

int
c_api_openxml(lua_State* L)
{
  if (l_xml != NULL)
  { return luaL_error(L, "ERROR: XML is already open in memory."); }

  XMLType xmltype = (XMLType)lua_tointeger(L, 1);
  lua_pop(L, 1);

  l_xml = xmlset(output, xmltype);

  return 1;
}

int
c_api_closexml(lua_State* L)
{
  if (l_xml == NULL)
  { return luaL_error(L, "ERROR: No XML open in memory."); }

  /* View XML file instead of compiling object. */
  if (compile == false)
  {
    lua_pushstring(L, l_xml->xml);
    xmlfree(l_xml);
    return 1;
  }

  FILE* stream = fopen(l_xml->path, "w");
  size_t i = 0;
  size_t length = strlen(l_xml->xml);
  for (; i<length; i++)
  {
    char c = l_xml->xml[i];

    switch (identation)
    {
      case -1: if ((c == '\n') || (c == '`')) { continue; }; break;
      case 1: if (c == '`') { fputs("  ", stream); continue; }; break;
      default: if (c == '`') { fputc('\t', stream); continue; }; break;
    }

    fputc(c, stream);
  }
  fclose(stream);

  xmlfree(l_xml);

  return 1;
}

