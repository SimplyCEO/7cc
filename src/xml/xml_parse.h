#if !defined(XML_PARSE_H)
# define XML_PARSE_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "xml.h"

XMLObject* xml_parse_fix_structure(XMLObject* xml);
XMLObject* xml_parse_identation(XMLObject* xml);
XMLObject* xml_parse_translate(XMLObject* xml, const char* n, const char* t);

# if defined(__cplusplus)
}
# endif

#endif

