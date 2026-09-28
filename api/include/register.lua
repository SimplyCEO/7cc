--  7cc: 7 Days to Die Code Compiler.
--  Copyright (C) 2026 SimplyCEO <simplyceo.developer@gmail.com>
--
--  This library is free software; you can redistribute it and/or modify it
--  under the terms of the GNU Library General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or (at your
--  option) any later version.
--
--  This library is distributed in the hope that it will be useful, but WITHOUT
--  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
--  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Library General Public
--  License for more details.
--
--  You should have received a copy of the GNU Library General Public License
--  along with this library; if not, see <https://www.gnu.org/licenses/>.

-- RETURNS INDEX OF ITEM FIELD
function register_item(properties)
  properties.property = properties.property or {}

  local index = 0
  local item =
  {
    name = properties.name,
    property =
    {
      { name = properties.property.name  or "HoldType",               value = properties.property.value or 45                                             },
      { name = properties.property.name  or "Tags",                   value = properties.property.value or "junk"                                         },
      { name = properties.property.name  or "Meshfile",               value = properties.property.value or "@:Other/Items/Misc/sackPrefab.prefab"         },
      { name = properties.property.name  or "DropMeshfile",           value = properties.property.value or "@:Other/Items/Misc/sack_droppedPrefab.prefab" },
      { name = properties.property.name  or "Material",               value = properties.property.value or "MresourceScraps"                              },
      { name = properties.property.name  or "Weight",                 value = properties.property.value or 5                                              },
      { name = properties.property.name  or "Stacknumber",            value = properties.property.value or 6000                                           },
      { name = properties.property.name  or "EconomicValue",          value = properties.property.value or 150                                            },
      { name = properties.property.name  or "Group",                  value = properties.property.value or "Resources"                                    },
      { name = properties.property.name  or "CraftingIngredientTime", value = properties.property.value or 0.2                                            },
      { name = properties.property.name  or "SoundPickup",            value = properties.property.value or "parts_grab"                                   },
      { name = properties.property.name  or "SoundPlace",             value = properties.property.value or "parts_place"                                  },
      { name = properties.property.name  or "CustomIcon",             value = properties.property.value or "meleeWpnBatonT2StunBatonParts"                },
      { name = properties.property.name  or "CustomIconTint",         value = properties.property.value or 674242                                         },
      { name = properties.property.name  or "DescriptionKey",         value = properties.property.value or "resourceScrapsDescription"                    }
    }
  }

  if (cc.get_architecture() == "32") then
    index = field.create("item", { name = item.name })
    for i=1, #item.property do field.add(index, field.create("property", item.property[i], true)) end
    return index
  end

  index = field.create("item", item)

  return index
end

-- REGISTER INDEX OF LOOT FIELD
function register_loot(properties)
  if (properties.item == nil) then return -1 end

  properties.item.count = properties.item.count or 1

  local xml_field_index = field.create("item", { name = properties.item.name, count = properties.item.count }, true)

  return field.append("lootcontainers/lootgroup[@name='group" .. properties.group .. "']", xml_field_index)
end

-- REGISTER INDEX OF RECIPE FIELD
function register_recipe(properties)
  if ((properties == nil) or (properties.ingredient == nil)) then return -1 end

  local index = 0
  local recipe =
  {
    name           = properties.name,
    count          = properties.count        or 1,
    craft_area     = properties.craft_area   or "workbench",
    craft_time     = properties.craft_time   or 10,
    craft_exp_gain = properties.craft_expain or 250,
    tags           = properties.tags         or "workbenchCrafting",
    ingredient     = properties.ingredient
  }

  index = field.create("recipe", recipe)

  return index
end

-- REGISTER INDEX OF SCHEMATIC FIELD
function register_schematic(properties)
  properties.property                      = properties.property                      or {}
  properties.effect_group                  = properties.effect_group                  or {}
  properties.effect_group.triggered_effect = properties.effect_group.triggered_effect or {}

  local index = 0
  local item =
  {
    name = properties.name .. "Schematic",
    property =
    {
      { name = properties.property.name or "Extends",      value = properties.property.value or "schematicNoQualityMaster" },
      { name = properties.property.name or "CreativeMode", value = properties.property.value or "Player"                   },
      { name = properties.property.name or "CustomIcon",   value = properties.property.value or properties.name            },
      { name = properties.property.name or "Unlocks",      value = properties.property.value or properties.name            }
    },
    effect_group =
    {
      tiered = properties.effect_group.tiered or false,
      triggered_effect =
      {
        {
          trigger   = properties.effect_group.trigger   or "onSelfPrimaryActionEnd",
          action    = properties.effect_group.action    or "ModifyCVar",
          cvar      = properties.effect_group.cvar      or "armorAthleticHelmet",
          operation = properties.effect_group.operation or "set",
          value     = properties.effect_group.value     or "1"
        },
        {
          trigger = properties.effect_group.trigger or "onSelfPrimaryActionEnd",
          action  = properties.effect_group.action  or "GiveExp",
          exp     = properties.effect_group.cvar    or "50"
        }
      }
    }
  }

  if (cc.get_architecture() == "32") then
    index = field.create("item", { name = item.name })

    local effect_group_index = field.create("effect_group", { tiered = item.effect_group.tiered })
    for i=1, 2 do field.add(effect_group_index, field.create("triggered_effect", item.effect_group.triggered_effect[i], true)) end
    field.add(index, effect_group_index)

    for i=1, #item.property do field.add(index, field.create("property", item.property[i], true)) end

    xml.close(effect_group_index)

    return index
  end

  index = field.create("item", item)

  return index
end

