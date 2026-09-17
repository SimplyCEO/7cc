#if !defined(XML_H)
# define XML_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "types.h"

typedef short XMLSize;

typedef struct XMLObject
{
  char*   path;
  char*   xml;
  XMLSize cursor;
  bool    garbage;
} XMLObject;

XMLObject* xml_object_free(XMLObject* xml_object);
XMLSize    xml_open(const char* path);
XMLObject* xml_get(const XMLSize index);
XMLObject* xml_set(const XMLSize index, const char* src);
XMLObject* xml_write(const XMLSize index, const char* src);
void       xml_close(const XMLSize index);

# if defined(__cplusplus)
}
# endif

#endif

