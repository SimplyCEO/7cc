#if !defined(XML_KEY_H)
# define XML_KEY_H

# if defined(__cplusplus)
extern "C"
{
# endif

# include "xml.h"

# define GAME_LOGIC         "event;name;type;icon;icon_color;icon_blink;count;range;min;max;checkresource;onslopes;prob;duration;rotatemax;stick_chance;"
# define GAME_ARCHETYPES    "male;race;variant;eye_color;"
# define GAME_BIOME         "biomemapcolor;lootstage_modifier;lootstage_bonus;gamestage_modifier;gamestage_bonus;difficulty;noise;noiseoffset;"
# define GAME_DECORATION    "prefab;blockname;"
# define GAME_ITEM_CRAFTING "craft_area;craft_time;craft_exp_gain;"
# define GAME_RESOURCE      "rwgGenerationType;"
# define GAME_TRIGGER       "trigger;action;name_key;description_key;message_key;buff;sound;"
# define GAME_LOGIC_GATE    "exp;cvar;operation;tags;value"

# define XMLKEY_DEFAULT_ORDER GAME_LOGIC \
                              GAME_ARCHETYPES \
                              GAME_BIOME \
                              GAME_DECORATION \
                              GAME_ITEM_CRAFTING \
                              GAME_RESOURCE \
                              GAME_TRIGGER \
                              GAME_LOGIC_GATE

typedef short XMLKeySize;

typedef struct XMLKey
{
  char* name;
  char* value;
} XMLKey;

XMLKeySize xml_key_size(XMLKey** xml_key);
XMLKeySize xml_key_search(XMLKey** xml_key, const char* key);
XMLKey**   xml_key_init(const XMLKeySize size);
XMLKey**   xml_key_expand(XMLKey** xml_key, const XMLKeySize new_size);
XMLKey*    xml_key_set(XMLKey* xml_key, const char* name, const char* value);
XMLKey**   xml_key_add(XMLKey** xml_key, const char* name, const char* value);
XMLKey**   xml_key_reorder(XMLKey** xml_key, const char* order);
XMLKey**   xml_key_free(XMLKey** xml_key);

# if defined(__cplusplus)
}
# endif

#endif

