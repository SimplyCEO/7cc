#if !defined(FCASEOPEN_H)
# define FCASEOPEN_H

# include <stdio.h>
# include "types.h"

# if defined(__cplusplus)
extern "C"
{
# endif

# if defined(_WIN32)
#   define fcaseopen fopen
#   define caserename rename
# else
const char* casepath(char const *path);
FILE* fcaseopen(char const *path, char const *mode);
void casechdir(char const *path);
# endif

# if defined(__cplusplus)
}
# endif
#endif

