#if !defined(XML_KEY_H)
# define XML_KEY_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "xml.h"

typedef struct XMLKey
{
  char* name;
  char* value;
} XMLKey;

size_t     xml_key_size(XMLKey** xml_key);
XMLKey**   xml_key_init(const size_t size);
XMLKey**   xml_key_expand(XMLKey** xml_key, const size_t new_size);
XMLKey*    xml_key_set(XMLKey* xml_key, const char* name, const char* value);
XMLKey**   xml_key_add(XMLKey** xml_key, const char* name, const char* value);
XMLKey**   xml_key_free(XMLKey** xml_key);

# if defined(__cplusplus)
}
# endif

#endif

