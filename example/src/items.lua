local items = {
  "armorNerdHelmetSchematic",
  "armorNerdOutfitSchematic",
  "armorNerdGlovesSchematic",
  "armorNerdBootsSchematic",
  "meleeWpnBatonT2StunBatonSchematic",
};

local function register_item(item, properties)
  return append(item, properties)
end

local function register_schematic(item)
  local formatted = item .. "Schematic"
  local properties = {
    extends = "schematicNoQualityMaster",
    creative_mode = "Player",
    custom_icon = item,
    unlocks = item
  }

  return append(formatted, properties)
end

openxml()

local length = #items
for i=1, length do
  local index = length - i + 1
  register_schematic(items[index])
end

local output = closexml()
-- print(output)

