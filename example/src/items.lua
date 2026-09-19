local xml_file = xml.open()

-- NAME ITEM ARRAY
local items = {
  "armorNerdHelmet",
  "armorNerdOutfit",
  "armorNerdGloves",
  "armorNerdBoots",
  "meleeWpnBatonT2StunBaton",
};

-- REGISTER `<item>...</item>` FIELD INDEX TEMPLATE
local function register_schematic(item_name)
  local properties = {
    index = field.create("item",
            {
              name = item_name .. "Schematic",
              property = {
                { name = "Extends", value = "schematicNoQualityMaster" },
                { name = "CreativeMode", value = "Player" },
                { name = "CustomIcon", value = item },
                { name = "Unlocks", value = item }
              },
              effect_group  = {
                tiered = false,
                triggered_effect = {
                  { trigger = "onSelfPrimaryActionEnd", action = "ModifyCVar", cvar = item, operation = "set", value = 1 },
                  { trigger = "onSelfPrimaryActionEnd", action = "GiveExp", exp = 50 }
                }
              }
            }),
    xml = nil
  }

  properties.xml = xml.get(properties.index)

  return properties
end

local schematics = { index = field.append("items", -1), xml = "" }
for i=1, #items do
  local schematic = register_schematic(items[i])
  schematics.xml = schematics.xml .. schematic.xml
  -- xml.close(schematic.index)
end

-- ADD GENERATED XML TO BACKEND XML OBJECT
field.add(schematics.index, schematics.xml)
-- xml.close(schematics.index)

-- ADD OBJECT XML TO FILE XML OBJECT
field.add(xml_file, schematics.index)
xml.close(xml_file)

-- INTERACTIVE MODE STRING (nil ONLY WHEN COMPILING OBJECT)
if (cc_output ~= nil) then
  print(cc_output)
end

