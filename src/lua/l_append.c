#include <stdio.h>
#include <string.h>

#include "l_append.h"
#include "l_xml.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "xml.h"

int          ibuffer = 0;
float        fbuffer = 0.0f;
const char*  sbuffer = NULL;

static char*
c_xml_properties(lua_State* L, const int mode, const char* name, const char* key)
{
  char* value = NULL;

  int type = lua_getfield(L, 2, key);

  if (type == LUA_TNIL)
  {
    lua_pop(L, 1);
    return NULL;
  }

  switch (mode)
  {
    case 0: ibuffer = lua_tointeger(L, -1); value = strdup(strfmt("value=\"%d\"", ibuffer)); break;
    case 1: fbuffer = lua_tonumber(L, -1); value = strdup(strfmt("value=\"%.1f\"", fbuffer)); break;
    case 2: sbuffer = lua_tostring(L, -1); value = strdup(strfmt("value=\"%s\"", sbuffer)); break;
    default: lua_pop(L, 1); return NULL;
  }

  lua_pop(L, 1);
  sbuffer = strfmt("\t\t\t<property name=\"%s\" %s/>", name, value);

  value = safe_free(value);
  value = strdup(sbuffer);

  return value;
}

int
c_api_append(lua_State* L)
{
  if (l_xml == NULL)
  { return luaL_error(L, "ERROR: No XML open in memory."); }

  char*        properties = NULL;
  char*        content    = NULL;
  size_t       cursor     = 0;
  size_t       size       = 0;

  int argc = lua_gettop(L);
  if (argc < 2)
  { return luaL_error(L, "usage: append(\"item\", properties)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "append(): First argument is not a valid string."); }

  if (lua_istable(L, 2) == false)
  { return luaL_error(L, "append(): Second argument is not a valid table."); }

  /*
   * <item name="resourceItem">
   * CONTENT_CURSOR_POSITION (to write properties)
   * </item>
   */
  sbuffer = strfmt("\t\t<item name=\"%s\">\n", lua_tostring(L, 1));
  cursor = strlen(sbuffer);
  size = cursor + strlen("\t\t</item>\n") + 1;
  content = safe_malloc(size*sizeof(char));
  strcpy(content, sbuffer);
  strcat(content, "\t\t</item>");

  properties = c_xml_properties(L, 2, "DescriptionKey", "description");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 0, "CustomIconTint", "custom_icon_tint");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "CustomIcon", "custom_icon");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "SoundPlace", "sound_place");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "SoundPickup", "sound_pickup");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 1, "CraftingIngredientTime", "crafting_time");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "Group", "group");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 0, "EconomicValue", "economic");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 0, "StackNumber", "stack");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 0, "Weight", "weight");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "Material", "material");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "DropMeshfile", "drop_mesh");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "Meshfile", "mesh");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "Tags", "tags");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 0, "HoldType", "hold_type");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "Extends", "extends");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "CreativeMode", "creative_mode");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  properties = c_xml_properties(L, 2, "Unlocks", "unlocks");
  content = strins(content, cursor, properties);
  properties = safe_free(properties);

  l_xml->xml = strins(l_xml->xml, l_xml->cursor, content);

  safe_free(content);

  return 1;
}

