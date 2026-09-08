#if !defined(C_API_XML_H)
# define C_API_XML_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "lua.h"
# include "xml.h"

int l_api_openxml(lua_State* L);
int l_api_closexml(lua_State* L);

extern XMLObject* l_xml;

# if defined(__cplusplus)
}
# endif

#endif

