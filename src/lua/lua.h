#if !defined(LUA_H)
# define LUA_H

# if defined(__cplusplus)
extern "C"
{
# endif

#define LUA_32BITS

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

const char* l_getvalue(lua_State* L, const int index);
lua_State*  l_init(void);
int         l_run(lua_State* L, const char* filepath);
int         l_free(lua_State* L);

# if defined(__cplusplus)
}
# endif

#endif

