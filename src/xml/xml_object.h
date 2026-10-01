#if !defined(XML_OBJECT_H)
# define XML_OBJECT_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "xml_typedef.h"

XMLSize    xml_object_size(XMLObject** object);
XMLObject* xml_object_alloc(const char* path);
XMLObject* xml_object_free(XMLObject* object);
XMLObject* xml_object_garbage(XMLObject* object);

# if defined(__cplusplus)
}
# endif

#endif

