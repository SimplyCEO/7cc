#if !defined(XML_FIELD_H)
# define XML_FIELD_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "xml_key.h"

XMLObject* xml_field_init(XMLObject* object, const char* field, XMLKey** values);
XMLObject* xml_field_add(XMLObject* object, XMLObject* field);

# if defined(__cplusplus)
}
# endif

#endif

