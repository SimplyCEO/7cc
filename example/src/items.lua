local items = {
  "armorNerdHelmet",
  "armorNerdOutfit",
  "armorNerdGloves",
  "armorNerdBoots",
  "meleeWpnBatonT2StunBaton",
};

local function register_schematic(item)
  local properties = {
    name = item .. "Schematic",
    property = {
      { name = "Extends", value = "schematicNoQualityMaster" },
      { name = "CreativeMode", value = "Player" },
      { name = "CustomIcon", value = item },
      { name = "Unlocks", value = item }
    },
    effect_group  = {
      tiered = false,
      triggered_effect = {
        { trigger = "onSelfPrimaryActionEnd", action = "ModifyCVar", cvar = "armorAthleticHelmet", operation = "set", value = "1" },
        { trigger = "onSelfPrimaryActionEnd", action = "GiveExp", exp = "50" }
      }
    }
  }

  return properties
end

local str_field = ""

local length = #items
for i=1, length do
  local index = length - i + 1
  local schematic = register_schematic(items[index])
  local field = cc.field_init("item", schematic)
  str_field = str_field .. field
end

cc.openxml()

cc.append("items", str_field)

cc.closexml()

if (xml ~= nil) then
  print(xml)
end

