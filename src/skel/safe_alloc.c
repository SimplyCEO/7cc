#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>

void*
safe_free(void* ptr)
{
  if (ptr != NULL)
  { free(ptr); }
  return NULL;
}

void*
safe_malloc(size_t size)
{
  void* ptr = malloc(size);

  if (ptr == NULL)
  {
    fprintf(stderr, "Memory allocation failed: %s\n", strerror(errno));
    return NULL;
  }

  memset(ptr, 0, size);

  return ptr;
}

void*
safe_calloc(size_t elements, size_t size)
{
  void* ptr = calloc(elements, size);

  if (ptr == NULL)
  {
    fprintf(stderr, "Memory allocation failed: %s\n", strerror(errno));
    return NULL;
  }

  return ptr;
}

void*
safe_realloc(void* ptr, size_t size)
{
  void* new_ptr = realloc(ptr, size);

  if (new_ptr == NULL)
  {
    fprintf(stderr, "Memory reallocation failed: %s\n", strerror(errno));
    return NULL;
  }

  return new_ptr;
}

