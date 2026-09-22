#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "toolbox.h"

#include "safe_alloc.h"
#include "types.h"

char* strfmt_ptr = NULL;

#if (__STDC_VERSION__ < 199901L) && !defined(__cplusplus)
static void
_tb_fillbuf(char* buffer, const size_t size)
{
  memset(buffer, '\0', size);
}

int32_t
_tb_vsprintf(char* src, const char* format, va_list ap)
{
  int count = 0;
  char* float_fmt = "%f";

  while (*format != '\0')
  {
    switch (*format)
    {
      case '%':
      {
        ++format;
        switch (*format)
        {
          case 'c': if (src != NULL) { *src++ = (char)va_arg(ap, int); } ++count; break;
          case 's':
          {
            char* s = va_arg(ap, char*);
            int s_len = 0;
            for (; *s!='\0'; ++s)
            { if (src != NULL) { *src++ = *s; } ++s_len; }
            count += s_len;
          } break;
          case 'd':
          {
            int num = va_arg(ap, int);
            char buffer[20] = {0}; _tb_fillbuf(&buffer[0], 20);
            int len = sprintf(buffer, "%d", num);
            int i = 0;
            for (; i<len; ++i)
            { if (src != NULL) { *src++ = buffer[i]; } ++count; }
          } break;
          case '.':
          {
            if ((format[1] >= '0') && (format[1] <= '9'))
            {
              float_fmt = safe_malloc(5*sizeof(char));
              sprintf(float_fmt, "%%.%cf", format[1]);
            }
            else
            {
              break;
            }
          }
          case 'f':
          {
            float num = (float)va_arg(ap, double);
            char buffer[20] = {0}; _tb_fillbuf(&buffer[0], 20);
            int len = sprintf(buffer, float_fmt, num);
            int i = 0;
            for (; i<len; ++i)
            { if (src != NULL) { *src++ = buffer[i]; } ++count; }

            if (strncmp(float_fmt, "%f", 2) != 0)
            {
              float_fmt = safe_free(float_fmt);
              float_fmt = "%f";
            }
          } break;
          case '%': if (src != NULL) { *src++ = '%'; } ++count; break;
          default: if (src != NULL) { *src++ = *format; } ++count; break;
        }
      } break;
      default: if (src != NULL) { *src++ = *format; } ++count; break;
    }
    ++format;
  }

  if (src != NULL)
  { *src = '\0'; }

  return count;
}
#endif

const char*
_tb_strfmt(const char* format, ...)
{
  strfmt_ptr = safe_free(strfmt_ptr);
  size_t fmt_size = 3072 + strlen(format);
  strfmt_ptr = safe_malloc(fmt_size*sizeof(char));

  va_list args;
  va_start(args, format);

  vsprintf(strfmt_ptr, format, args);
  va_end(args);

  strfmt_ptr = safe_realloc(strfmt_ptr, (strlen(strfmt_ptr)+1)*sizeof(char));

  return strfmt_ptr;
}

/* Duplicate given string memory contents. */
char*
_tb_strdup(const char* str)
{
  if (str == NULL)
  { return NULL; }

  size_t size = strlen(str) + 1;
  char* ptr = safe_malloc(size*sizeof(char));

  return (char*)memcpy(ptr, str, size);
}

/* Remove strict character after given position. */
char*
_tb_strsub(char* src, const size_t pos, const char c)
{
  size_t i = 0;
  size_t size = strlen(src) + 1;
  char* ptr = safe_malloc(size*sizeof(char));
  char* ptr_start = ptr;

  char* str = src;
  for (i=0; *str!='\0'; i++)
  {
    if ((i>pos) && (*str == c))
    { str++; continue; }
    *ptr++ = *str++;
  }
  *ptr = '\0';

  /* Return ptr to initial position. */
  ptr = ptr_start;

  src = safe_free(src);
  src = ptr;

  return src;
}

char*
_tb_strlwr(char* src)
{
  char* output = src;
  char* str = strdup(src);
  char* ptr = str;

  while (*str != '\0')
  { *src++ = tolower(*str++); }
  *src = '\0';

  safe_free(ptr);

  return output;
}

char*
_tb_strupr(char* src)
{
  char* output = src;
  char* str = strdup(src);
  char* ptr = str;

  while (*str != '\0')
  { *src++ = toupper(*str++); }
  *src = '\0';

  safe_free(ptr);

  return output;
}

/* Insert string after given position. */
char*
_tb_strins(char* src, const size_t pos, const char* ins)
{
  if (ins == NULL)
  { return src; }

  size_t src_len = strlen(src);
  size_t ins_len = strlen(ins);
  size_t size = src_len + ins_len + 1;

  char* ptr = safe_malloc(size*sizeof(char));

  if (pos > 0)
  {
    strncpy(ptr, src, pos);
    ptr[pos] = '\0';
  }

  strcat(ptr, ins);

  strcat(ptr, src + pos);

  src = safe_free(src);
  src = ptr;

  return src;
}

/* Copy contents of string using given positions. */
char*
_tb_strcut(char* src, const size_t n1, const size_t n2)
{
  size_t size = n2 - n1 + 1;
  char* ptr = safe_malloc(size*sizeof(char));

  strncpy(ptr, src + n1, size);
  ptr[size] = '\0';

  src = safe_free(src);
  src = ptr;

  return src;
}

/* Delete contents of string using given positions. */
char*
_tb_strdel(char* src, const size_t n1, const size_t n2)
{
  size_t size = strlen(src) + 1;
  char* ptr = safe_malloc(size*sizeof(char));

  strncpy(ptr, src, n1);
  strncat(ptr, src + n2, size + n2);

  src = safe_free(src);
  src = ptr;

  return src;
}

char*
_tb_basename(const char* path)
{
  if (path == NULL)
  { return NULL; }

  const char* slash = strrchr(path, '/');
  const char* backslash = strrchr(path, '\\');
  const char* separator = (slash > backslash) ? slash : backslash;

  if (separator == NULL)
  { return (char*)path; }

  return (char*)(separator+1);
}

char*
_tb_dirname(const char* path)
{
  if (path == NULL)
  { return NULL; }

  static char result[256] = {0};

  strcpy(result, path);
  char* delimiter = strchr(result, '\\');
  if (delimiter == NULL)
  {
    delimiter = strchr(result, '/');
    if (delimiter == NULL)
    { return result; }
  }

  const size_t path_len = strlen(path);
  const size_t path_base_len = strlen(_tb_basename(path));
  const size_t eos = (path_len == path_base_len) ? path_len - 1 : path_len - path_base_len - 1;

  strncpy(result, path, eos);
  result[eos] = '\0';

  return result;
}

bool
_tb_iffile(const char* path)
{
  FILE* stream = fopen(path, "rb");
  if (stream == NULL)
  { return false; }
  fclose(stream);

  return true;
}

bool
_tb_ifdir(const char* path)
{
  struct stat statbuf;
  if (stat(path, &statbuf) != 0)
  { return false; }

  return ((S_ISDIR(statbuf.st_mode) == 0) ? false : true);
}

