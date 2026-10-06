#if !defined(LUA_GENERATE_H)
# define LUA_GENERATE_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "lua.h"

# include "xml_key.h"
# include "xml_object.h"

XMLKey**   l_api_generate_key(lua_State* L, const int index);
XMLObject* l_api_generate_field(lua_State* L, const int index, const char* field);

# if defined(__cplusplus)
}
# endif

#endif

