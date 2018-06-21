#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "nfc3d/amiibo.h"
#include "amiibo_lua.h"

#define LIB_LUA
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#ifndef LUA_VERSION_NUM
#define luaL_Reg luaL_reg
#endif

/* Crypto keys stored here */
static nfc3d_amiibo_keys retail_keys;


static int l_unpack (lua_State *L) {
  /* Amiibo tag dump buffer must be supplied */
  size_t tag_sz;
  const char *tag = luaL_checklstring(L, 1, &tag_sz);

  luaL_argcheck(L, tag_sz >= NFC3D_AMIIBO_SIZE, 1, "Buffer must be correct size");

  /* Decrypt and unpack the tag dump */
  char *unpacked = malloc(NFC3D_AMIIBO_SIZE);
  if (unpacked == NULL) {
    return 0;
  }

  nfc3d_amiibo_unpack(&retail_keys, (uint8_t*) tag, (uint8_t*) unpacked);

  /* Return unpacked Amiibo data */
  lua_pushlstring(L, unpacked, NFC3D_AMIIBO_SIZE);
  free(unpacked);
  return 1;
}

static int l_pack (lua_State *L) {
  /* Amiibo unpacked buffer must be supplied */
  size_t plain_sz;
  const char *plain = luaL_checklstring(L, 1, &plain_sz);

  luaL_argcheck(L, plain_sz == NFC3D_AMIIBO_SIZE, 1, "Buffer must be correct size");

  /* Pack the Amiibo */
  char *packed = malloc(NFC3D_AMIIBO_SIZE);
  if (packed == NULL) {
    return 0;
  }

  nfc3d_amiibo_pack(&retail_keys, (uint8_t*) plain, (uint8_t*) packed);

  /* Return packed Amiibo data */
  lua_pushlstring(L, packed, NFC3D_AMIIBO_SIZE);
  free(packed);
  return 1;
}

static int l_load_keys (lua_State *L) {
  /* A file path for the key must be set */
  const char* path = luaL_checklstring(L, 1, NULL);

  /* Attempt to load keys and return result */
  bool result = nfc3d_amiibo_load_keys(&retail_keys, path);
  lua_pushboolean(L, result);

  return 1;
}

/* Table of library functions */
static const struct luaL_Reg luamiibo[] = {
  {"unpack", l_unpack},
  {"pack", l_pack},
  {"load_keys", l_load_keys},
  {NULL, NULL}
};

/* Register the library */
int luaopen_luamiibo (lua_State *L) {
  luaL_newlib(L, luamiibo);
  return 1;
}
