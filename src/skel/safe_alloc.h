#if !defined(SAFE_ALLOC_H)
# define SAFE_ALLOC_H

# if defined(__cplusplus)
extern "C"
{
# endif

void* safe_free(void* ptr);
void* safe_malloc(size_t size);
void* safe_calloc(size_t elements, size_t size);
void* safe_realloc(void* ptr, size_t size);

# if defined(__cplusplus)
}
# endif

#endif

