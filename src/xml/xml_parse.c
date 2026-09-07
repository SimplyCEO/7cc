#include <string.h>

#include "xml_parse.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

XMLObject*
xml_parse_fix_structure(XMLObject* xml)
{
  if (xml == NULL)
  { return NULL; }

  int i = 0;
  int b = 0;
  int open = -1;
  int close = -1;
  int length = strlen(xml->xml) + 1;
  bool opened = false;
  bool closed = false;
  bool single_field = false;
  bool reset = false;
  char c = 0;
  char* buffer = safe_malloc(length*sizeof(char));

  for(; i<length; ++i)
  {
    if (reset == true)
    { reset = false; opened = false; closed = false; single_field = false; open = -1; close = -1; }

    c = xml->xml[i];

    switch (c)
    {
      case '<':
      {
        if (opened == false)
        {
          closed = false;
          opened = true;

          if ((open == -1) && (xml->xml[i+1] == '/')) { reset = true; break; }

          if      (open == -1)            { open = i; }
          else if (single_field == false) { close = i; }
          /* <field/> */
          else                            { reset = true; break; }

          if ((open > -1) && (close > -1))
          {
            int step = 0;
            bool skip_field = false;
            while (true)
            {
              ++step;

              if ((xml->xml[open+step] == ' ') || (xml->xml[open+step] == '>'))
              { skip_field = true; break; }

              if (xml->xml[open+step] != xml->xml[close+1+step])
              { break; }
            }
            reset = true;
            if (skip_field == true)
            { ++b; buffer[b-2] = '/'; buffer[b-1] = '>'; i += step + 1; continue; }
            i = close - 1;

            continue;
          }
        }
      } break;
      case '>':
      {
        if (closed == false)
        {
          opened = false;
          closed = true;

          if (xml->xml[i-1] == '/') { single_field = true; }
        }
      } break;
      case '?': reset = true; break;
      default: break;
    }

    buffer[b] = c;
    ++b;
  }

  buffer = safe_realloc(buffer, (strlen(buffer)+1)*sizeof(char));
  xml->xml = safe_free(xml->xml);
  xml->xml = buffer;

  return xml;
}

XMLObject*
xml_parse_identation(XMLObject* xml)
{
  if (xml == NULL)
  { return NULL; }

  int i = 0;
  int b = 0;
  int length = strlen(xml->xml) + 1;
  int identation = -1;
  char c = 0;
  char* buffer = safe_malloc((length+1024)*sizeof(char));

  for(; i<length; ++i)
  {
    c = xml->xml[i];

    switch (c)
    {
      case '<': if (xml->xml[i+1] == '?') { if (xml->xml[i+1] != '/') { ++identation; } --identation; } break;
      case '>':
      {
        buffer[b] = c;
        ++b;
        buffer[b] = ',';
        ++b;

        if (xml->xml[i-1] == '?') { --identation; buffer[b] = ','; ++b; }
        if (xml->xml[i-1] == '/') { --identation; }
        if (xml->xml[i+2] == '/') { --identation; }

        ++identation;
        int t = 0;
        for (; t<=identation; ++t)
        {
          buffer[b] = '.';
          ++b;
        }

        if (xml->xml[i+2] == '/') { --identation; }
      } break;
      default: break;
    }

    switch (buffer[b-1])
    {
      case ',': case '.': ++i; c = xml->xml[i]; break;
    }

    buffer[b] = c;
    ++b;
  }

  size_t buffer_length = strlen(buffer);
  for (i=buffer_length; buffer[i]!='>'; --i)
  {
    c = buffer[i];

    switch (c) { case ',': case '.': buffer[i] = '\0'; }
  }

  buffer = safe_realloc(buffer, (strlen(buffer)+1)*sizeof(char));
  xml->xml = safe_free(xml->xml);
  xml->xml = buffer;

  return xml;
}

XMLObject*
xml_parse_translate(XMLObject* xml, const char* n, const char* t)
{
  if (xml == NULL)
  { return NULL; }

  int i = 0;
  int b = 0;
  int length = strlen(xml->xml) + 1;
  char c = 0;
  bool replace = false;
  char* buffer = safe_malloc((length+1024)*sizeof(char));

  for(; i<length; ++i)
  {
    c = xml->xml[i];

    if (replace == true)
    {
      size_t r = 0;
      switch (c)
      {
        case ',': for (; r<strlen(n); ++r) { buffer[b] = n[r]; ++b; } continue;
        case '.': for (; r<strlen(t); ++r) { buffer[b] = t[r]; ++b; } continue;
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
  xml->xml = safe_free(xml->xml);
  xml->xml = buffer;

  return xml;
}

