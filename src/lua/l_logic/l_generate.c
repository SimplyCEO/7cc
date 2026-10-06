#include <string.h>

#include "l_generate.h"

#include "xml_field.h"
#include "xml_key.h"
#include "xml_object.h"

#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"

#if (BUILD64 == 0)
XMLSize xml32_index = 0;
#endif
const char* buffer = NULL;

static char*
_strbuff(lua_State* L, const int index)
{
  static char output[64] = {0};

  memset(output, '\0', 64);
  strncpy(output, l_getvalue(L, index), 63);

  return output;
}

static char*
_strdupfree(char* src, const char* buffer)
{
  src = safe_free(src);
  src = strdup(buffer);

  return src;
}

XMLKey**
l_api_generate_key(lua_State* L, const int index)
{
  char*    name  = NULL;
  char*    value = NULL;
  XMLKey** keys  = xml_key_init(1);

  if (lua_istable(L, index) == true)
  {
    lua_pushnil(L);
    while (lua_next(L, index) != 0)
    {
      int sub_index = lua_gettop(L);

      /* Currently there is no support for array keys. */
      if (lua_type(L, sub_index - 1) == LUA_TNUMBER)
      {
#if (BUILD64 == 0)
        xml32_index = -32;
#endif
        lua_pop(L, 2);
        break;
      }

      if (lua_istable(L, sub_index) == false)
      {
        /* Ignore NULL values even if key exists. */
        value = _strdupfree(value, _strbuff(L, sub_index));

        if (strncmp(value, "nil", 3) != 0)
        {
          name = _strdupfree(name, _strbuff(L, sub_index - 1));
          keys = xml_key_add(keys, name, value);
        }
      }

      lua_pop(L, 1);
    }
  }

  name = safe_free(name);
  value = safe_free(value);

  return keys;
}

XMLObject*
l_api_generate_field(lua_State* L, const int index, const char* field)
{
  int        i            = 0;
  int        table_length = 0;
  char*      name         = NULL;
  char*      value        = NULL;
  XMLObject* object       = xml_object_alloc();

  object->data->keys = l_api_generate_key(L, index);

#if (BUILD64 == 0)
  if (xml32_index == -32)
  {
    object = xml_object_free(object);
    return object;
  }
#endif

  if (buffer == NULL)
  { buffer = field; }

  /* Generate field with keys. */
  object->data->keys = xml_key_reorder(object->data->keys, XMLKEY_DEFAULT_ORDER);
  object = xml_field_init(object, field, object->data->keys);

  do
  {
    /* Generate sub field if found in table. */
    if (lua_istable(L, index) == true)
    {
      lua_len(L, index);
      bool is_table_array = ((table_length = (int)lua_tointeger(L, -1)) != 0);
      lua_pop(L, 1);

      if (is_table_array == true)
      {
        object = xml_object_free(object);
        object = xml_object_alloc();

        /* Iterate table array. */
        for (i=table_length; i>=1; --i)
        {
          lua_rawgeti(L, index, i);

          XMLObject* field_object = xml_object_alloc();
          field_object->data->keys = l_api_generate_key(L, lua_gettop(L));

          field_object->data->keys = xml_key_reorder(field_object->data->keys, XMLKEY_DEFAULT_ORDER);
          field_object = xml_field_init(field_object, buffer, field_object->data->keys);

          object = xml_field_add(object, field_object);

          lua_pop(L, 1);
        }

        break;
      }

      /* lua_next() can not receive numeric keys. */
      lua_pushnil(L);
      while (lua_next(L, index) != 0)
      {
        int sub_index = lua_gettop(L);

        if (strncmp((value = _strdupfree(value, _strbuff(L, sub_index))), "table", 3) == 0)
        {
          buffer = name = _strdupfree(name, _strbuff(L, sub_index - 1));
          XMLObject* field_object = l_api_generate_field(L, sub_index, name);

#if (BUILD64 == 0)
          if (field_object->garbage == true)
          {
            lua_pop(L, 2);
            break;
          }
#endif

          object = xml_field_add(object, field_object);
          buffer = NULL;
        }

        lua_pop(L, 1);
      }
    }
  }
  while (0);

  name = safe_free(name);
  value = safe_free(value);

  return object;
}

