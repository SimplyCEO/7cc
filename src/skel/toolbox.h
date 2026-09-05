#if !defined(TOOLBOX_H)
# define TOOLBOX_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include <stdarg.h>

# include "types.h"

# if (__STDC_VERSION__ < 199901L) && !defined(__cplusplus)
int32_t _vsprintf(char* src, const char* format, va_list ap);
#   define vsprintf _vsprintf
# endif

const char* strfmt(const char* format, ...);
char* _strdup(const char* str);
char* _strsub(char* src, const size_t pos, const char c);
char* _strlwr(char* src);
char* _strupr(char* src);
char* _strins(char* src, const size_t pos, const char* ins);
char* _strcut(char* src, const size_t n1, const size_t n2);
char* _strdel(char* src, const size_t n1, const size_t n2);
char* _basename(const char* path);
char* _dirname(const char* path);
bool iffile(const char* path);
bool ifdir(const char* path);
bool ifsymlink(const char* path);

#define strdup _strdup
#define strsub _strsub
#define strlwr _strlwr
#define strupr _strupr
#define strins _strins
#define strcut _strcut
#define strdel _strdel
#define basename _basename
#define dirname _dirname

# if defined(__cplusplus)
}
# endif

#endif

