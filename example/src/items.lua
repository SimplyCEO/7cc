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
                { name = "CustomIcon", value = item_name },
                { name = "Unlocks", value = item_name }
              },
              effect_group  = {
                tiered = false,
                triggered_effect = {
                  { trigger = "onSelfPrimaryActionEnd", action = "ModifyCVar", cvar = item_name, operation = "set", value = 1 },
                  { trigger = "onSelfPrimaryActionEnd", action = "GiveExp", exp = 50 }
                }
              }
            }),
    xml = nil
  }

  if (cc.get_architecture() == "32") then
    field.add(properties.index, field.create("property", { name = "Extends", value = "schematicNoQualityMaster" }))
    field.add(properties.index, field.create("property", { name = "CreativeMode", value = "Player" }))
    field.add(properties.index, field.create("property", { name = "CustomIcon", value = item_name }))
    field.add(properties.index, field.create("property", { name = "Unlocks", value = item_name }))

    local effect_group = field.create("effect_group", { tiered = false })
    field.add(effect_group, field.create("triggered_effect", { trigger = "onSelfPrimaryActionEnd", action = "ModifyCVar", cvar = item_name, operation = "set", value = 1 }))
    field.add(effect_group, field.create("triggered_effect", { trigger = "onSelfPrimaryActionEnd", action = "GiveExp", exp = 50 }))

    field.add(properties.index, effect_group)
  end

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

