#if !defined(XML_TYPEDEF_H)
# define XML_TYPEDEF_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "types.h"

typedef int XMLSize;
typedef int XMLKeySize;

typedef struct XMLKey
{
  char* name;
  char* value;
} XMLKey;

typedef struct XMLData
{
  char*    content;
  XMLKey** keys;
} XMLData;

typedef struct XMLInfo
{
  char*   path;
  char*   field;
  XMLSize size;
} XMLInfo;

typedef struct XMLObject
{
  XMLData*           data;
  XMLInfo*           info;
  struct XMLObject** field;
  bool               garbage;
  bool               index;
  bool               print;
} XMLObject;

# if defined(__cplusplus)
}
# endif

#endif

