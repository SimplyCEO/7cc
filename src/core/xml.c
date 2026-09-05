#include <stdio.h>
#include <string.h>

#include "xml.h"

#include "safe_alloc.h"
#include "toolbox.h"

XMLObject*
xmlset(const char* path, XMLType type)
{
  char*       buffer    = NULL;
  const char* type_name = NULL;
  XMLObject*  xml       = NULL;

  switch (type)
  {
    case ENTITYCLASSES: type_name = "entityclasses"; break;
    case ITEMS:         type_name = "items";         break;
    case LOOT:          type_name = "loot";          break;
    case MATERIALS:     type_name = "materials";     break;
    case RECIPES:       type_name = "recipes";       break;
    default: break;
  }

  if (xml == NULL)
  {
    xml = (XMLObject*)safe_malloc(sizeof(XMLObject));
  }

  buffer = safe_malloc(1024*sizeof(char));

  const char* dir_path = dirname(path);
  if ((dir_path == NULL) || (dir_path[0] == '\0') || (strcmp(dir_path, path) == 0))
  { strcpy(buffer, strfmt("%s.xml", type_name)); }
  else
  { strcpy(buffer, strfmt("%s/%s.xml", dir_path, type_name)); }
  xml->path = strdup(buffer);

  strcpy(buffer, strfmt("<%s>\n`<append xpath=\"/%s\">\n", type_name, type_name));
  xml->cursor = strlen(buffer);
  strcat(buffer, strfmt("`</append>\n</%s>", type_name));

  xml->xml = strdup(buffer);

  safe_free(buffer);

  return xml;
}

void
xmlfree(XMLObject* xml)
{
  if (xml != NULL)
  {
    xml->path = safe_free(xml->path);
    xml->xml = safe_free(xml->xml);
    xml = safe_free(xml);
  }
}

