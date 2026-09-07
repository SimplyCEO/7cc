#include <stdio.h>
#include <string.h>

#include "l_xml.h"

#include "xml.h"
#include "xml_parse.h"

#include "main.h"

XMLObject* l_xml = NULL;

int
c_api_openxml(lua_State* L)
{
  if (l_xml != NULL)
  { return luaL_error(L, "ERROR: XML is already open in memory."); }

  l_xml = xml_init(output);
  l_xml = xml_version(l_xml);

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
    xml_free(l_xml);
    return 1;
  }

  l_xml = xml_parse_fix_structure(l_xml);
  l_xml = xml_parse_identation(l_xml);

  switch (identation)
  {
    case -1: l_xml = xml_parse_translate(l_xml, "", ""); break;
    case 1: l_xml = xml_parse_translate(l_xml, "\n", "  "); break;
    default: l_xml = xml_parse_translate(l_xml, "\n", "\t"); break;
  }

  FILE* stream = fopen(l_xml->path, "w");
  size_t i = 0;
  for (; i<strlen(l_xml->xml); i++)
  { fputc(l_xml->xml[i], stream); }
  fclose(stream);

  l_xml = xml_free(l_xml);

  return 1;
}

