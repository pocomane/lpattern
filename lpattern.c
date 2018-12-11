
/******************************************************************************
* This is free and unencumbered software released into the public domain.
* 
* Anyone is free to copy, modify, publish, use, compile, sell, or
* distribute this software, either in source code form or as a compiled
* binary, for any purpose, commercial or non-commercial, and by any
* means.
* 
* In jurisdictions that recognize copyright laws, the author or authors
* of this software dedicate any and all copyright interest in the
* software to the public domain. We make this dedication for the benefit
* of the public at large and to the detriment of our heirs and
* successors. We intend this dedication to be an overt act of
* relinquishment in perpetuity of all present and future rights to this
* software under copyright law.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
* 
* For more information, please refer to <http://unlicense.org/>
******************************************************************************/

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lpattern.h"

// Mask unused lua API

#define LUAMOD_API 
#define LUA_INTEGER_FRMLEN ""
#define LUA_INTEGER_FMT ""
#define LUA_NUMBER_FRMLEN ""
#define LUA_MININTEGER (0)
#define LUA_TNIL (1)
#define LUA_TFUNCTION (3)
#define LUA_TNUMBER (4)
#define LUA_TSTRING (5)
#define LUA_TTABLE (6)
#define LUA_TBOOLEAN (7)
#define MAX_10_EXP (0)

#define l_mathlim(...) (0)
#define l_sprintf(...) (0)

#define luaL_addchar(...) (0)
#define luaL_addlstring(...) (0)
#define luaL_addsize(...) (0)
#define luaL_addstring(...) (0)
#define luaL_addvalue(...) (0)
#define luaL_argcheck(...) (0)
#define luaL_argcheck(...) (0)
#define luaL_argerror(...) (0)
#define luaL_buffinit(...) (0)
#define luaL_buffinitsize(...) (0)
#define luaL_checkinteger(...) (0)
#define luaL_checklstring(...) (0)
#define luaL_checknumber(...) (0)
#define luaL_checkstack(...) (0)
#define luaL_checkstring(...) (0)
#define luaL_checktype(...) (0)
#define luaL_checktype(...) (0)
#define luaL_error(...) (0)
#define luaL_newlib(...) (0)
#define luaL_optinteger(...) (0)
#define luaL_optinteger(...) (0)
#define luaL_optlstring(...) (0)
#define luaL_optlstring(...) (0)
#define luaL_prepbuffsize(...) (0)
#define luaL_prepbuffsize(...) (0)
#define luaL_pushresult(...) (0)
#define luaL_pushresultsize(...) (0)
#define luaL_tolstring(...) (0)
#define luaL_typename(...) (0)
#define lua_Integer int
#define lua_Number int
#define lua_Toboolean(...) (0)
#define lua_assert(...) (0)
#define lua_call(...) (0)
#define lua_createtable(...) (0)
#define lua_dump(...) (0)
#define lua_getlocaledecpoint(...) (0)
#define lua_gettable(...) (0)
#define lua_gettop(...) (0)
#define lua_isinteger(...) (0)
#define lua_isstring(...) (0)
#define lua_newuserdata(...) (0)
#define lua_number2strx(...) (0)
#define lua_pop(...) (0)
#define lua_pushcclosure(...) (0)
#define lua_pushinteger(...) (0)
#define lua_pushliteral(...) (0)
#define lua_pushlstring(...) (0)
#define lua_pushnil(...) (0)
#define lua_pushnumber(...) (0)
#define lua_pushvalue(...) (0)
#define lua_remove(...) (0)
#define lua_setfield(...) (0)
#define lua_setmetatable(...) (0)
#define lua_settop(...) (0)
#define lua_toboolean(...) (0)
#define lua_tointeger(...) (0)
#define lua_tolstring(...) (0)
#define lua_tolstring(...) (0)
#define lua_tonumber(...) (0)
#define lua_touserdata(...) (0)
#define lua_type(...) (0)
#define lua_upvalueindex(...) (0)
#define num2straux(...) (0)

#define luaL_Buffer int
#define lua_State int
#define lua_Unsigned int

typedef int luafunc(lua_State *);
typedef struct {char* a; luafunc* b;} luaL_Reg;

// Original lua string library

#include "lstrlib.c"

// WRAPPER

lpattern_t lpattern_find (const char* p, size_t lp, const char* s, size_t ls) {
  lua_State Ld; 
  lua_State *L = &Ld; 
  lpattern_t result = {0};
  int init = 1;
  MatchState ms;

  const char *s1 = s + init - 1;
  int anchor = (*p == '^');
  if (anchor) {
    p++; lp--;  /* skip anchor character */
  }

  prepstate(&ms, L, s, ls, p, lp);
  do {
    const char *res;
    reprepstate(&ms);
    if ((res=match(&ms, s1, p)) != NULL) {
      result.found = s1;
      result.length = res -s1;
      break;
    }
  } while (s1++ < ms.src_end && !anchor);

  result.level = 0;
  for (int i=0; i<ms.level; i++){
    if (CAP_POSITION != ms.capture[i].len && CAP_UNFINISHED != ms.capture[i].len) {
      // TODO : error if any CAP_UNFINISHED ?
      result.level += 1;
      result.capture[i].data = ms.capture[i].init;
      result.capture[i].length = ms.capture[i].len;
    }
  }
  return result;
}

