#include <string.h>

#include "xml_key.h"

#include "safe_alloc.h"
#include "toolbox.h"

XMLObject*
xml_field_add(XMLObject* xml, const char* name, const XMLKey** key)
{
  if (xml == NULL)
  { return NULL; }

  char* keys = safe_malloc(sizeof(char));

  if (key != NULL)
  {
    if (*key != NULL)
    {
      int i = 0;
      int size = 0;

      while (key[i] != NULL)
      { ++i; }
      size = i;

      /* Insert keys backwards. */
      for (i=(i-1); i>=0; --i)
      { keys = strins(keys, 0, strfmt(" %s=\"%s\"", key[i]->name, key[i]->value)); }
    }
  }

  xml->xml = safe_free(xml->xml);
  xml->xml = strdup(strfmt("<%s%s></%s>", name, keys, name));
  xml->cursor = strlen(xml->xml) - strlen(name) - 3;

  safe_free(keys);

  return xml;
}

