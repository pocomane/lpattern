
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
#include <setjmp.h>

#include "lpattern.h"

// Mask unused lua API

#define lua_assert(...) (0)
#define luaL_error(S,...) (snprintf(S->result->extra.error_message,LPATTERN_MAX_ERROR,__VA_ARGS__), S->result->is_error = 1, longjmp (S->buf, 1), 0)
#define lua_Integer int

typedef struct {
  lpattern_t* result;
  jmp_buf buf;
} lua_State;

typedef int luafunc(lua_State *);
typedef struct {char* a; luafunc* b;} luaL_Reg;

// Original lua string library

#include "lstrlib.c"

// WRAPPER

lpattern_t lpattern_find (const char* p, size_t lp, const char* s, size_t ls) {
  lpattern_t result = {0};

  lua_State Ld; 
  lua_State *L = &Ld; 
  L->result = &result;
  if (setjmp(L->buf)) {
    return result;
  }

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
      result.extra.capture[i].data = ms.capture[i].init;
      result.extra.capture[i].length = ms.capture[i].len;
    }
  }
  return result;
}

char* lpattern_error (lpattern_t* state){
  if (!state || !state->is_error) return 0;
  return state->extra.error_message;
}

