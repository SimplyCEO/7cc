#include <stdlib.h>
#include <string.h>

#include "xml_object.h"
#include "xml_key.h"

#include "safe_alloc.h"
#include "toolbox.h"

XMLObject*
xml_object_version(XMLObject* object)
{
  object->data->content = strdup("?xml version=\"1.0\" encoding=\"UTF-8\"?");
  object->info->size = strlen(object->data->content);

  return object;
}

XMLSize
xml_object_size(XMLObject** object)
{
  XMLSize size = 0;

  while (object[size]!=NULL) { ++size; }

  return size;
}

XMLObject*
xml_object_alloc(void)
{
  XMLObject* object = (XMLObject*)malloc(sizeof(XMLObject));

  object->data = (XMLData*)malloc(sizeof(XMLData));
  object->data->content = NULL;
  object->data->keys = NULL;

  object->info = (XMLInfo*)malloc(sizeof(XMLInfo));
  object->info->path = NULL;
  object->info->field = NULL;
  object->info->size = 0;

  object->field = NULL;

  object->garbage = false;
  object->index = false;
  object->print = false;

  return object;
}

XMLObject*
xml_object_free(XMLObject* object)
{
  if (object != NULL)
  {
    if (object->field != NULL)
    {
      XMLSize i = 0;

      for (; object->field[i]!=NULL; ++i)
      {
        if (object->field[i]->index == true) { continue; }
        object->field[i] = xml_object_free(object->field[i]);
      }

      object->field = safe_free(object->field);
    }

    object->data->content = safe_free(object->data->content);
    object->data->keys = xml_key_free(object->data->keys);
    object->data = safe_free(object->data);

    object->info->path = safe_free(object->info->path);
    object->info->field = safe_free(object->info->field);
    object->info->size = 0;
    object->info = safe_free(object->info);

    object = safe_free(object);
  }

  return object;
}

XMLObject*
xml_object_garbage(XMLObject* object)
{
  if (object != NULL)
  {
    if (object->field != NULL)
    {
      XMLSize i = 0;
      for (; object->field[i]!=NULL; ++i) { object->field[i] = xml_object_free(object->field[i]); }
      object->field = safe_free(object->field);
    }

    object->data->content = safe_free(object->data->content);
    object->data->keys = xml_key_free(object->data->keys);

    object->info->path = safe_free(object->info->path);
    object->info->field = safe_free(object->info->field);
    object->info->size = 0;

    object->garbage = true;
  }

  return object;
}

