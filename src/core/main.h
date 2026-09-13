#if !defined(MAIN_H)
# define MAIN_H

# if defined(__cplusplus)
extern "C"
{
# endif

# define PROJECT_MAJOR 0
# define PROJECT_MINOR 0
# define PROJECT_PATCH 10

# include "types.h"

extern bool compile;
extern char* output;
extern int identation;

# if defined(__cplusplus)
}
# endif

#endif

