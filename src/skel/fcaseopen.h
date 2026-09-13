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
char* casepath(const char* path);
FILE* fcaseopen(const char* path, const char* mode);
void casechdir(const char* path);
# endif

# if defined(__cplusplus)
}
# endif
#endif

