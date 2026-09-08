#include <stdlib.h>

#include "xml_key.h"

#include "safe_alloc.h"
#include "toolbox.h"

size_t
xml_key_size(XMLKey** xml_key)
{
  size_t size = 0;

  while (xml_key[size] != NULL) { ++size; }

  return size;
}

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

XMLKey**
xml_key_expand(XMLKey** xml_key, const size_t new_size)
{
  size_t i = 0;
  size_t size = xml_key_size(xml_key);

  /* TODO: Shrink the structure. */
  if ((size == new_size) || (size > new_size)) { return xml_key; }

  XMLKey** new_xml_key = (XMLKey**)realloc(xml_key, (new_size+1)*sizeof(XMLKey*));

  if (new_xml_key == NULL) { return xml_key; }
  xml_key = new_xml_key;

  for (i=size; i<new_size; ++i)
  {
    xml_key[i] = (XMLKey*)malloc(sizeof(XMLKey));
    xml_key[i]->name = NULL;
    xml_key[i]->value = NULL;
  }
  xml_key[new_size] = NULL;

  return xml_key;
}

XMLKey*
xml_key_set(XMLKey* xml_key, const char* name, const char* value)
{
  xml_key->name = safe_free(xml_key->name);
  xml_key->value = safe_free(xml_key->value);

  xml_key->name = strdup(name);
  xml_key->value = strdup(value);

  return xml_key;
}

XMLKey**
xml_key_add(XMLKey** xml_key, const char* name, const char* value)
{
  if (xml_key == NULL)
  { return NULL; }

  if (xml_key[0]->value == NULL)
  {
    xml_key[0] = xml_key_set(xml_key[0], name, value);
    return xml_key;
  }

  size_t size = xml_key_size(xml_key);

  xml_key = xml_key_expand(xml_key, (size+1));

  xml_key[size] = xml_key_set(xml_key[size], name, value);

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

