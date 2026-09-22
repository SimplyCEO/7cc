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
#   define vsprintf _tb_vsprintf
# endif

const char* _tb_strfmt(const char* format, ...);
char*       _tb_strdup(const char* str);
char*       _tb_strsub(char* src, const size_t pos, const char c);
char*       _tb_strlwr(char* src);
char*       _tb_strupr(char* src);
char*       _tb_strins(char* src, const size_t pos, const char* ins);
char*       _tb_strcut(char* src, const size_t n1, const size_t n2);
char*       _tb_strdel(char* src, const size_t n1, const size_t n2);
char*       _tb_basename(const char* path);
char*       _tb_dirname(const char* path);
bool        _tb_iffile(const char* path);
bool        _tb_ifdir(const char* path);
  
#define strfmt _tb_strfmt
#define strdup _tb_strdup
#define strsub _tb_strsub
#define strlwr _tb_strlwr
#define strupr _tb_strupr
#define strins _tb_strins
#define strcut _tb_strcut
#define strdel _tb_strdel
#define basename _tb_basename
#define dirname _tb_dirname
#define iffile _tb_iffile
#define ifdir _tb_ifdir

# if defined(__cplusplus)
}
# endif

#endif

