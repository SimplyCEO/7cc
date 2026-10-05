-- SYSTEM LUA API
doinclude("register.lua")

-- NAME ITEM ARRAY
local items = { "armorNerdHelmet", "armorNerdOutfit", "armorNerdGloves", "armorNerdBoots", "meleeWpnBatonT2StunBaton" }

local schematics = { index = field.append("items", -1), xml = "" }

-- ADD GENERATED FIELD TO BACKEND FIELD OBJECT
for i=1, #items do
  local index = register_schematic({ name = items[#items - i + 1] })
  field.add(schematics.index, index)
  xml.close(index)
end

-- ADD OBJECT FIELD TO FILE FIELD OBJECT
field.add(0, schematics.index)

-- CLOSE ALL FIELDS AND PRINT XML FILE (EMPTY ONLY WHEN COMPILING OBJECT)
cc.exit()

