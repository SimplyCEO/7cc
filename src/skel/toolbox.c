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
int32_t
_vsprintf(char* src, const char* format, va_list ap)
{
  int count = 0;

  while (*format != '\0')
  {
    switch (*format)
    {
      case '%':
      {
        format++;
        switch (*format)
        {
          case 'c':
          {
            if (src != NULL)
            { *src++ = (char)va_arg(ap, int); }
            count++;
          } break;
          case 's':
          {
            char* s = va_arg(ap, char*);
            int s_len = 0; for (; *s != '\0'; s++)
            {
              if (src != NULL)
              { *src++ = *s; }
              s_len++;
            }
            count += s_len;
          } break;
          case 'd':
          {
            int num = va_arg(ap, int);
            char buffer[20] = {0};
            int len = sprintf(buffer, "%d", num);
            int i = 0; for (; i < len; i++)
            {
              if (src != NULL)
              { *src++ = buffer[i]; }
              count++;
            }
          } break;
          case '%':
          {
            if (src != NULL)
            { *src++ = '%'; }
            count++;
          } break;
          default:
          {
            if (src != NULL)
            { *src++ = *format; }
            count++;
          } break;
        }
      } break;
      default:
      {
        if (src != NULL)
        { *src++ = *format; }
        count++;
      } break;
    }
    format++;
  }
  if (src != NULL)
  { *src = '\0'; }

  return count;
}
#endif

const char*
strfmt(const char* format, ...)
{
  strfmt_ptr = safe_free(strfmt_ptr);

  va_list args;
  va_start(args, format);

  strfmt_ptr = safe_malloc(1024*sizeof(char));
  vsprintf(strfmt_ptr, format, args);
  va_end(args);

  strfmt_ptr = safe_realloc(strfmt_ptr, (strlen(strfmt_ptr)+1)*sizeof(char));

  return strfmt_ptr;
}

/* Duplicate given string memory contents. */
char*
_strdup(const char* str)
{
  if (str == NULL)
  { return NULL; }

  size_t size = strlen(str) + 1;
  char* ptr = safe_malloc(size*sizeof(char));

  return (char*)memcpy(ptr, str, size);
}

/* Remove strict character after given position. */
char*
_strsub(char* src, const size_t pos, const char c)
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

  src = safe_realloc(src, (strlen(ptr)+1)*sizeof(char));
  strcpy(src, ptr);

  safe_free(ptr);

  return src;
}

char*
_strlwr(char* src)
{
  char* str = strdup(src);
  char* ptr = str;

  while (*str != '\0')
  { *src++ = tolower(*str++); }
  *src = '\0';

  safe_free(ptr);

  return src;
}

char*
_strupr(char* src)
{
  char* str = strdup(src);
  char* ptr = str;

  while (*str != '\0')
  { *src++ = toupper(*str++); }
  *src = '\0';

  safe_free(ptr);

  return src;
}

/* Insert string after given position. */
char*
_strins(char* src, const size_t pos, const char* ins)
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
  src = strdup(ptr);

  safe_free(ptr);

  return src;
}

/* Copy contents of string using given positions. */
char*
_strcut(char* src, const size_t n1, const size_t n2)
{
  size_t size = n2 - n1 + 1;
  char* ptr = safe_malloc(size*sizeof(char));

  strncpy(ptr, src + n1, size);

  src = safe_realloc(src, size*sizeof(char));
  strcpy(src, ptr);

  safe_free(ptr);

  return src;
}

/* Delete contents of string using given positions. */
char*
_strdel(char* src, const size_t n1, const size_t n2)
{
  size_t size = strlen(src) + 1;
  char* ptr = safe_malloc(size*sizeof(char));

  strncpy(ptr, src, n1);
  strncat(ptr, src + n2, strlen(src+n2) + 1);
  src = safe_realloc(src, (strlen(ptr)+1)*sizeof(char));
  strcpy(src, ptr);

  safe_free(ptr);

  return src;
}

char*
_basename(const char* path)
{
  if (path == NULL)
  { return NULL; }

  const char *slash = strrchr(path, '/');
  const char *backslash = strrchr(path, '\\');
  const char *separator = (slash > backslash) ? slash : backslash;

  if (separator == NULL)
  { return (char*)path; }

  return (char*)(separator+1);
}

char*
_dirname(const char* path)
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

  const size_t eos = strlen(path) - strlen(_basename(path)) - 1;

  strncpy(result, path, eos);
  result[eos] = '\0';

  return result;
}

bool
iffile(const char* path)
{
  FILE* stream = fopen(path, "rb");
  if (stream == NULL)
  { return true; }
  fclose(stream);

  return false;
}

bool
ifdir(const char* path)
{
  struct stat statbuf;
  if (stat(path, &statbuf) != 0)
  { return true; }

  return ((S_ISDIR(statbuf.st_mode) == 0) ? true : false);
}

bool
ifsymlink(const char* path)
{
  struct stat statbuf;
  if (lstat(path, &statbuf) < 0)
  { return false; }

  return ((S_ISLNK(statbuf.st_mode) == 0) ? true : false);
}

