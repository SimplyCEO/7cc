#if !defined(MAIN_H)
# define MAIN_H

# if defined(__cplusplus)
extern "C"
{
# endif

# define PROJECT_MAJOR 0
# define PROJECT_MINOR 0
# define PROJECT_PATCH 15
# define PROJECT_7CC_VERSION ((PROJECT_MAJOR*100000)+(PROJECT_MINOR*1000)+(PROJECT_PATCH))

# include "types.h"

extern bool compile;
extern char* output;
extern int identation;

# if defined(__cplusplus)
}
# endif

#endif

