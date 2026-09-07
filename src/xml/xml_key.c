#include <stdlib.h>

#include "xml_key.h"

#include "safe_alloc.h"
#include "toolbox.h"

XMLKey**
xml_key_init(const size_t size)
{
  size_t i = 0;

  XMLKey** xml_key = (XMLKey**)malloc((size+1)*sizeof(XMLKey*));

  for (; i<size; ++i)
  {
    xml_key[i] = (XMLKey*)malloc(sizeof(XMLKey));
    xml_key[i]->name = NULL;
    xml_key[i]->value = NULL;
  }
  xml_key[size] = NULL;

  return xml_key;
}

XMLKey*
xml_key_set(XMLKey* xml_key, const char* name, const char* value)
{
  xml_key->name = strdup(name);
  xml_key->value = strdup(value);

  return xml_key;
}

XMLKey**
xml_key_free(XMLKey** xml_key)
{
  if (xml_key != NULL)
  {
    size_t i = 0;
    while (xml_key[i] != NULL)
    {
      xml_key[i]->name = safe_free(xml_key[i]->name);
      xml_key[i]->value = safe_free(xml_key[i]->value);
      xml_key[i] = safe_free(xml_key[i]);
    }
    xml_key = safe_free(xml_key);
  }

  return NULL;
}

