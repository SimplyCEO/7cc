#include <stdlib.h>
#include <string.h>

#include "xml_key.h"

#include "safe_alloc.h"
#include "toolbox.h"

XMLKeySize
xml_key_size(XMLKey** object)
{
  XMLKeySize size = 0;

  while (object[size] != NULL) { ++size; }

  return size;
}

XMLKeySize
xml_key_search(XMLKey** object, const char* key)
{
  XMLKeySize i = 0;
  XMLKeySize index = -1;

  for (; object[i]!=NULL; ++i)
  {
    size_t length = 0;
    for (; key[length]!='\0'; ++length) { if (key[length+1] == ';') { break; } }

    if (strncmp(object[i]->name, key, length) == 0)
    { index = i; break; }
  }

  return index;
}

XMLKey**
xml_key_init(const XMLKeySize size)
{
  XMLKeySize i = 0;

  XMLKey** object = (XMLKey**)malloc((size+1)*sizeof(XMLKey*));

  for (; i<size; ++i)
  {
    object[i] = (XMLKey*)malloc(sizeof(XMLKey));
    object[i]->name = NULL;
    object[i]->value = NULL;
  }
  object[size] = NULL;

  return object;
}

XMLKey**
xml_key_expand(XMLKey** object, const XMLKeySize new_size)
{
  XMLKeySize i = 0;
  XMLKeySize size = xml_key_size(object);

  /* TODO: Shrink the structure. */
  if ((size == new_size) || (size > new_size)) { return object; }

  object = (XMLKey**)safe_realloc(object, (new_size+1)*sizeof(*object));

  for (i=size; i<new_size; ++i)
  {
    object[i] = (XMLKey*)malloc(sizeof(XMLKey));
    object[i]->name = NULL;
    object[i]->value = NULL;
  }
  object[new_size] = NULL;

  return object;
}

XMLKey*
xml_key_set(XMLKey* object, const char* name, const char* value)
{
  object->name = safe_free(object->name);
  object->value = safe_free(object->value);

  object->name = strdup(name);
  object->value = strdup(value);

  return object;
}

XMLKey**
xml_key_add(XMLKey** object, const char* name, const char* value)
{
  if (object == NULL)
  { return NULL; }

  if (object[0]->value == NULL)
  {
    object[0] = xml_key_set(object[0], name, value);
    return object;
  }

  XMLKeySize size = xml_key_size(object);

  object = xml_key_expand(object, (size+1));

  object[size] = xml_key_set(object[size], name, value);

  return object;
}

XMLKey**
xml_key_reorder(XMLKey** object, const char* order)
{
  if ((object == NULL) || ((*object)->name == NULL))
  { return object; }

  XMLKeySize  i = 0;
  XMLKeySize  n = 0;
  XMLKeySize  size = 0;
  XMLKeySize  n_size = 0;
  XMLKeySize* indexes = NULL;
  XMLKey**    new_object = NULL;
  const char* order_ptr = order;

  size = xml_key_size(object);
  new_object = xml_key_init(size);

  indexes = safe_malloc(size*sizeof(XMLKeySize));
  for (i=0; i<size; ++i)
  { indexes[i] = -1; }

  for (i=0; order_ptr[i]!='\0'; ++i)
  {
    if ((order_ptr[i] == ';') || (i == 0))
    {
      if (order_ptr[i] == ';') { ++i; }
      if (order_ptr[i] == '\0') { break; }

      /* -1 means given key do not exist in `object`. */
      XMLKeySize index = xml_key_search(object, order_ptr + i);
      if (index != -1)
      {
        indexes[n_size] = index;
        ++n_size;
      }
    }
  }

  /* index missing keys. */
  if (size != n_size)
  {
    XMLKeySize* avoid = safe_malloc(size*sizeof(XMLKeySize));
    for (i=0; i<size; ++i)
    { avoid[i] = 0; }

    for (n=0; n<=size; ++n)
    {
      for (i=0; object[i]!=NULL; ++i)
      {
        if (avoid[i] == 1) { continue; }

        if      (indexes[n] == i) { avoid[i] = 1; break; }
        else if (indexes[n] == -1) { indexes[n_size] = i; ++n_size; avoid[i] = 1; break; }

        if (size == n_size) { break; }
      }
      if (size == n_size) { break; }
    }

    avoid = safe_free(avoid);
  }

  /* reorder keys. */
  if (size == n_size)
  {
    for (i=0; new_object[i]!=NULL; ++i)
    {
      new_object[i] = safe_free(new_object[i]);
      new_object[i] = object[indexes[i]];
    }
  }

  indexes = safe_free(indexes);
  object = safe_free(object);

  return new_object;
}

XMLKey**
xml_key_free(XMLKey** object)
{
  if (object != NULL)
  {
    XMLKeySize i = 0;
    while (object[i] != NULL)
    {
      object[i]->name = safe_free(object[i]->name);
      object[i]->value = safe_free(object[i]->value);
      object[i] = safe_free(object[i]);
    }
    object = safe_free(object);
  }

  return NULL;
}

