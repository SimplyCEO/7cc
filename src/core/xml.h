#if !defined(XML_H)
# define XML_H

# if defined(__cplusplus)
extern "C"
{
# endif

typedef struct XMLObject
{
  char* path;
  char* xml;
  size_t cursor;
} XMLObject;

typedef enum XMLType
{
  ENTITYCLASSES = 0,
  ITEMS,
  LOOT,
  MATERIALS,
  RECIPES
} XMLType;

XMLObject* xmlset(const char* path, XMLType type);
void xmlfree(XMLObject* xml);

extern XMLObject* entityclasses;
extern XMLObject* items;
extern XMLObject* loot;
extern XMLObject* materials;
extern XMLObject* recipes;

# if defined(__cplusplus)
}
# endif

#endif

