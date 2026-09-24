#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

FILE*
fopen64(const char* path, const char* mode)
{
  return fopen(path, mode);
}

FILE*
freopen64(const char* path, const char* mode, FILE* stream)
{
  return freopen(path, mode, stream);
}

FILE*
tmpfile64(void)
{
  return tmpfile();
}

int
fseeko64(FILE* stream, off_t offset, int whence)
{
  return fseek(stream, offset, whence);
}

off_t
ftello64(FILE* stream)
{
  return ftell(stream);
}

int
mkstemp64(char* template)
{
  return mkstemp(template);
}

