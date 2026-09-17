#if !defined(C_API_XML_H)
# define C_API_XML_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "lua.h"
# include "xml.h"

const char* l_api_xml(lua_State* L);

extern XMLSize    l_xml_index;
extern XMLObject* l_xml;

# if defined(__cplusplus)
}
# endif

#endif

