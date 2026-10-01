#include <string.h>

#include "xml_parse.h"

#include "safe_alloc.h"
#include "toolbox.h"

#define TAB '+'
#define NL '`'

int tab_size = 0;

XMLObject*
xml_parse_translate(XMLObject* object, const char* t, const char* n)
{
  if (object == NULL)
  { return NULL; }

  int i = 0;
  int b = 0;
  int length = strlen(object->data->content) + 1;
  char c = 0;
  bool replace = false;
  char* buffer = safe_malloc((length*2)*sizeof(char));

  for(; i<length; ++i)
  {
    c = object->data->content[i];

    if (replace == true)
    {
      size_t r = 0;
      switch (c)
      {
        case TAB: for (; r<strlen(t); ++r) { buffer[b] = t[r]; ++b; } continue;
        case NL:  for (; r<strlen(n); ++r) { buffer[b] = n[r]; ++b; } continue;
      }
    }

    switch (c)
    {
      case '<': replace = false; break;
      case '>': replace = true; break;
    }

    buffer[b] = c;
    ++b;
  }

  buffer = safe_realloc(buffer, (strlen(buffer)+1)*sizeof(char));
  object->data->content = safe_free(object->data->content);
  object->data->content = buffer;
  object->info->size = strlen(object->data->content);

  return object;
}

XMLObject*
xml_parse_assemble(XMLObject* object)
{
  if (object == NULL) { return NULL; }
  if (object->data == NULL) { return NULL; }

  char* buffer = NULL;
  char* identation = safe_malloc((tab_size+1)*sizeof(char));

  int i = 0;
  for (; i<tab_size; ++i)
  { identation[i] = TAB; }

  ++tab_size;

  if (object->field == NULL)
  { buffer = strdup(strfmt("%s<%s/>%c", identation, object->data->content, NL)); }
  else
  {
    XMLSize size = 0;
    char* xml_content = safe_malloc(sizeof(char));

    for (; object->field[size]!=NULL; ++size)
    {
      if (object->field[size]->garbage == true)
      { continue; }
      xml_content = strins(xml_content, strlen(xml_content), xml_parse_assemble(object->field[size])->data->content);
    }

    buffer = strdup(strfmt("%s<%s>%c%s%s</%s>%c", identation, object->data->content, NL, xml_content, identation, object->info->field, NL));

    xml_content = safe_free(xml_content);
  }

  object->data->content = safe_free(object->data->content);
  object->data->content = buffer;
  object->info->size = strlen(object->data->content);

  --tab_size;

  safe_free(identation);

  return object;
}

