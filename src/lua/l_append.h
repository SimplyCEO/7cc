#if !defined(C_API_APPEND_H)
# define C_API_APPEND_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "lua.h"

int c_api_append(lua_State* L);

# if defined(__cplusplus)
}
# endif

#endif

