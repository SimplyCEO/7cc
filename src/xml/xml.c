#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xml.h"

#include "safe_alloc.h"
#include "toolbox.h"

XMLObject** xml = NULL;

static XMLObject*
xml_object_version(XMLObject* xml_object)
{
  xml_object->xml = strdup("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
  xml_object->cursor = strlen(xml_object->xml);

  return xml_object;
}

static XMLObject*
xml_object_alloc(const char* path)
{
  XMLObject* xml_object = (XMLObject*)malloc(sizeof(XMLObject));
  xml_object->path = safe_malloc(sizeof(char));
  xml_object->xml = safe_malloc(sizeof(char));
  xml_object->cursor = 0;
  xml_object->garbage = false;

  if (path != NULL)
  {
    char*       buffer   = strdup(path);
    const char* dir_path = dirname(path);

    /* Directory is either empty or a file name. */
    if ((dir_path[0] == '\0') || (strcmp(dir_path, path) == 0))
    {
      strcpy(buffer, basename(path));
    }

    xml_object->path = safe_free(xml_object->path);
    xml_object->path = strdup(buffer);
    buffer = safe_free(buffer);

    xml_object = xml_object_version(xml_object);
  }

  return xml_object;
}

XMLObject*
xml_object_free(XMLObject* xml_object)
{
  if (xml_object != NULL)
  {
    xml_object->path = safe_free(xml_object->path);
    xml_object->xml = safe_free(xml_object->xml);
    xml_object->cursor = 0;
    xml_object->garbage = true;
  }

  return xml_object;
}

XMLSize
xml_open(const char* path)
{
  if (xml != NULL)
  {
    XMLSize i = 0;

    for (; xml[i]!=NULL; ++i)
    {
      if (xml[i]->garbage == true)
      {
        xml[i] = safe_free(xml[i]);
        xml[i] = xml_object_alloc(path);
        return i;
      }
    }

    XMLSize size = i + 2;
    xml = (XMLObject**)realloc(xml, size*sizeof(XMLObject*));

    for (; i<size-1; ++i)
    { xml[i] = xml_object_alloc(path); }
    xml[i] = NULL;

    return i-1;
  }

  xml = (XMLObject**)malloc(2*sizeof(XMLObject*));
  xml[0] = xml_object_alloc(path);
  xml[1] = NULL;

  return 0;
}

XMLObject*
xml_get(const XMLSize index)
{
  return xml[index];
}

XMLObject*
xml_set(const XMLSize index, const char* src)
{
  xml[index]->xml = safe_free(xml[index]->xml);
  xml[index]->xml = strdup(src);
  xml[index]->cursor = strlen(src);

  return xml[index];
}

XMLObject*
xml_write(const XMLSize index, const char* src)
{
  xml[index]->xml = strins(xml[index]->xml, xml[index]->cursor, src);

  return xml[index];
}

void
xml_close(const XMLSize index)
{
  if (index == -1)
  {
    XMLSize i = 0;
    for (; xml[i]!=NULL; ++i)
    {
      xml[i] = xml_object_free(xml[i]);
      xml[i] = safe_free(xml[i]);
    }
    xml = safe_free(xml);
    return;
  }
  xml[index] = xml_object_free(xml[index]);
}

