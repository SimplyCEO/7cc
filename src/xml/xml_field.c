#include <stdlib.h>
#include <string.h>

#include "xml_field.h"

#include "safe_alloc.h"
#include "toolbox.h"

XMLObject*
xml_field_init(XMLObject* object, const char* field, XMLKey** values)
{
  if ((object == NULL) || (object->info->size > 0))
  { return object; }

  char* keys = safe_malloc(sizeof(char));

  if ((values != NULL) && ((*values)->value != NULL))
  {
    if (*values != NULL)
    {
      int i = 0;

      for (i=0; values[i]!=NULL; ++i)
      {
        keys = strins(keys, strlen(keys), strfmt("%s=\"%s\" ", values[i]->name, values[i]->value));
      }
    }
  }

  object->data->content = strdup(strfmt("%s %s", field, keys));
  object->info->field = strdup(field);
  object->info->size = strlen(object->data->content);

  safe_free(keys);

  return object;
}

XMLObject*
xml_field_add(XMLObject* object, XMLObject* field)
{
  if ((object == NULL) || (field == NULL))
  { return object; }

  int i = 0;
  bool skip = false;

  if (object->field == NULL)
  {
    object->field = (XMLObject**)malloc(2*sizeof(*object->field));
    object->field[0] = field;
    object->field[1] = NULL;

    return object;
  }

  for (i=0; object->field[i]!=NULL; ++i)
  {
    if (object->field[i]->garbage == true)
    {
      skip = true;
      break;
    }
  }

  if (skip == false)
  {
    int size = i + 2;
    object->field = (XMLObject**)safe_realloc(object->field, size*sizeof(*object->field));

    for (; i<size-1; ++i)
    { object->field[i] = field; }
    object->field[i] = NULL;

    return object;
  }

  object->field[i] = field;

  return object;
}

