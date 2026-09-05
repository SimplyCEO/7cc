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

int l_init(void);
int l_run(const char* filepath);
int l_free(void);

# if defined(__cplusplus)
}
# endif

#endif

