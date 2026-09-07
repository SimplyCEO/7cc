#if !defined(XML_FIELD_H)
# define XML_FIELD_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "xml_key.h"

XMLObject* xml_add_field(XMLObject* xml, const char* name, const XMLKey** key);

# if defined(__cplusplus)
}
# endif

#endif

