#include <stdio.h>
#include <string.h>

#include "l_append.h"
#include "l_xml.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "xml.h"
#include "xml_field.h"
#include "xml_key.h"

int          ibuffer = 0;
float        fbuffer = 0.0f;
const char*  sbuffer = NULL;

static XMLObject*
c_xml_properties(XMLObject* xml, lua_State* L, const int mode, const char* name, const char* key)
{
  xml->xml = safe_free(xml->xml);

  if (lua_getfield(L, 2, key) == LUA_TNIL)
  {
    lua_pop(L, 1);
    return xml;
  }

  const char* value = NULL;

  switch (mode)
  {
    case 0: value = strfmt("%d", lua_tointeger(L, -1)); break;
    case 1: value = strfmt("%.1f", lua_tonumber(L, -1)); break;
    case 2: value = strfmt("%s", lua_tostring(L, -1)); break;
    default: lua_pop(L, 1); return xml;
  }

  lua_pop(L, 1);

  XMLKey** xml_key = xml_key_init(2);
  xml_key[0] = xml_key_set(xml_key[0], "name", name);
  xml_key[1] = xml_key_set(xml_key[1], "value", value);

  xml = xml_field_add(xml, "property", (const XMLKey**)xml_key);

  xml_key = xml_key_free(xml_key);

  return xml;
}

int
c_api_append(lua_State* L)
{
  if (l_xml == NULL)
  { return luaL_error(L, "ERROR: No XML open in memory."); }

  int argc = lua_gettop(L);
  if (argc < 2)
  { return luaL_error(L, "usage: append(\"item\", properties)"); }

  if (lua_isstring(L, 1) == false)
  { return luaL_error(L, "append(): First argument is not a valid string."); }

  if (lua_istable(L, 2) == false)
  { return luaL_error(L, "append(): Second argument is not a valid table."); }

  XMLObject* l_append = xml_init(NULL);
  XMLKey** l_append_key = xml_key_init(1);
  l_append_key[0] = xml_key_set(l_append_key[0], "name", lua_tostring(L, 1));
  l_append = xml_field_add(l_append, "append", (const XMLKey**)l_append_key);
  l_append_key = xml_key_free(l_append_key);

  XMLObject* properties = xml_init(NULL);

  properties = c_xml_properties(properties, L, 2, "DescriptionKey", "description");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 0, "CustomIconTint", "custom_icon_tint");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "CustomIcon", "custom_icon");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "SoundPlace", "sound_place");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "SoundPickup", "sound_pickup");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 1, "CraftingIngredientTime", "crafting_time");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "Group", "group");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 0, "EconomicValue", "economic");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 0, "StackNumber", "stack");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 0, "Weight", "weight");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "Material", "material");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "DropMeshfile", "drop_mesh");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "Meshfile", "mesh");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "Tags", "tags");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 0, "HoldType", "hold_type");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "Extends", "extends");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "CreativeMode", "creative_mode");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  properties = c_xml_properties(properties, L, 2, "Unlocks", "unlocks");
  l_append->xml = strins(l_append->xml, l_append->cursor, properties->xml);

  l_xml->xml = strins(l_xml->xml, l_xml->cursor, l_append->xml);

  properties = xml_free(properties);
  l_append = xml_free(l_append);

  return 1;
}

