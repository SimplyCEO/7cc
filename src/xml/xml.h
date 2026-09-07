#if !defined(XML_H)
# define XML_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include <stdio.h>

typedef struct XMLObject
{
  char* path;
  char* xml;
  size_t cursor;
} XMLObject;

XMLObject* xml_init(const char* path);
XMLObject* xml_version(XMLObject* xml);
XMLObject* xml_free(XMLObject* xml);

# if defined(__cplusplus)
}
# endif

#endif

