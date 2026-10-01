#include <stdlib.h>

#include "xml.h"
#include "xml_object.h"

#include "safe_alloc.h"
#include "toolbox.h"

XMLObject** xml = NULL;

XMLSize
xml_open(void)
{
  if (xml != NULL)
  {
    XMLSize i = 0;

    for (; xml[i]!=NULL; ++i)
    {
      if (xml[i]->garbage == true)
      { return i; }
    }

    XMLSize size = i + 2;
    xml = (XMLObject**)safe_realloc(xml, size*sizeof(*xml));

    for (; i<size-1; ++i)
    { xml[i] = xml_object_alloc(); }
    xml[i] = NULL;

    return i-1;
  }

  xml = (XMLObject**)malloc(2*sizeof(XMLObject*));
  xml[0] = xml_object_alloc();
  xml[1] = NULL;

  return 0;
}

XMLObject*
xml_get(const XMLSize index)
{
  return xml[index];
}

XMLSize
xml_set(XMLObject* object, XMLSize index)
{
  XMLSize size = xml_object_size(xml);

  if ((index > size) || (index == 0))
  { index = xml_open(); }

  xml[index] = xml_object_free(xml[index]);
  xml[index] = object;

  return index;
}

XMLObject*
xml_write(const XMLSize index, const char* src)
{
  xml[index]->data->content = strins(xml[index]->data->content, xml[index]->info->size, src);

  return xml[index];
}

void
xml_close(const XMLSize index)
{
  if (index == -1)
  {
    XMLSize i = 0;
    for (; xml[i]!=NULL; ++i)
    { xml[i] = xml_object_free(xml[i]); }
    xml = safe_free(xml);
    return;
  }

  xml[index] = xml_object_garbage(xml[index]);
}

