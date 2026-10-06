#if !defined(XML_H)
# define XML_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "xml_object.h"
# include "xml_typedef.h"

XMLSize    xml_open(void);
XMLSize    xml_add(XMLObject* object);
XMLObject* xml_get(const XMLSize index);
XMLSize    xml_set(XMLObject* object, XMLSize index);
XMLObject* xml_write(const XMLSize index, const char* src);
void       xml_close(const XMLSize index);

# if defined(__cplusplus)
}
# endif

#endif

