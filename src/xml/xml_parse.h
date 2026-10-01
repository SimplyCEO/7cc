#if !defined(XML_PARSE_H)
# define XML_PARSE_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "xml.h"

XMLObject* xml_parse_translate(XMLObject* object, const char* n, const char* t);
XMLObject* xml_parse_assemble(XMLObject* object);

# if defined(__cplusplus)
}
# endif

#endif

