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
  local xml_field = {
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
  }

  local properties = { index = nil, xml = nil }

  if (cc.get_architecture() == "32") then
    properties.index = field.create("item", { name = item_name .. "Schematic" })

    local effect_group = field.create("effect_group", { tiered = false })
    for i=1, 2 do field.add(effect_group, field.create("triggered_effect", xml_field.effect_group.triggered_effect[i], true)) end
    field.add(properties.index, effect_group)

    for i=1, 4 do field.add(properties.index, field.create("property", xml_field.property[i], true)) end
  end

  if (properties.index == nil) then
    properties.index = field.create("item",
      {
        name = item_name .. "Schematic",
        property = xml_field.property,
        effect_group = xml_field.effect_group
      }
    )
  end

  properties.xml = xml.get(properties.index)

  return properties
end

local schematics = { index = field.append("items", -1), xml = "" }
for i=1, #items do
  local schematic = register_schematic(items[i])
  schematics.xml = schematics.xml .. schematic.xml
  xml.close(schematic.index)
end

-- ADD GENERATED XML TO BACKEND XML OBJECT
field.add(schematics.index, schematics.xml)
-- ADD OBJECT XML TO FILE XML OBJECT
field.add(0, schematics.index)

xml.close(schematics.index)
xml.close(0)

-- INTERACTIVE MODE STRING (nil ONLY WHEN COMPILING OBJECT)
if (cc_output ~= nil) then
  print(cc_output)
end

