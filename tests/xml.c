#include <stdio.h>

#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"
#include "xml_object.h"
#include "xml_parse.h"

int
main(void)
{
  XMLObject* object = xml_object_alloc();

  object = xml_field_init(object, "example_1", NULL);

  object = xml_field_add(object, xml_field_init(xml_object_alloc(), "field_0", NULL));
  object = xml_field_add(object, xml_field_add(xml_field_init(xml_object_alloc(), "field_1", NULL), xml_field_init(xml_object_alloc(), "field_0", NULL)));
  object = xml_field_add(object, xml_field_init(xml_object_alloc(), "field_2", NULL));

  object = xml_parse_assemble(object);
  object = xml_parse_translate(object, "  ", "\n");

  printf("%s", object->data->content);

  object = xml_object_free(object);

  if (object != NULL)
  { return 1; }

  return 0;
}
