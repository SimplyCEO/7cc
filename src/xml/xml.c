#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xml.h"

#include "safe_alloc.h"
#include "toolbox.h"

/* <?xml version="1.0" encoding="UTF-8"?><type><!-- CURSOR --></type> */

XMLObject*
xml_init(const char* path)
{
  XMLObject* xml = (XMLObject*)malloc(sizeof(XMLObject));
  xml->path = NULL;
  xml->xml = NULL;
  xml->cursor = 0;

  if (path != NULL)
  {
    size_t      path_len = strlen(path);
    char*       buffer   = strdup(path);
    const char* dir_path = dirname(path);

    /* Directory is either empty or a file name. */
    if ((dir_path[0] == '\0') || (strcmp(dir_path, path) == 0))
    {
      strcpy(buffer, basename(path));
    }

    xml->path = strdup(buffer);
    buffer = safe_free(buffer);
  }

  return xml;
}

XMLObject*
xml_version(XMLObject* xml)
{
  xml->xml = strdup("<?xml version=\"1.00\" encoding=\"UTF-8\"?>");
  xml->cursor = strlen(xml->xml);

  return xml;
}

XMLObject*
xml_free(XMLObject* xml)
{
  if (xml != NULL)
  {
    xml->path = safe_free(xml->path);
    xml->xml = safe_free(xml->xml);
    xml = safe_free(xml);
  }

  return NULL;
}

