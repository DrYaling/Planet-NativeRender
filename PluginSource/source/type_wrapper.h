#pragma once
/*
rust 基本数据类型
*/
#include <stdint.h>
#include "Logger/Logger.h"
#include "LuaBinder/tolua.h"
struct Position{
    int32_t x;
    int32_t y;
};
struct AiHookState {
    uint32_t state_id;
    ///执行状态,0-succss,1-fail,2-running
    int32_t state;
};
typedef logger::CPPLogCallback UnityLogCallback;
typedef void(UNITY_INTERFACE_API* StateHookCallback)(uint64_t, AiHookState);
typedef void(UNITY_INTERFACE_API* NetworkEventCallback)(int32_t, uint32_t, const char*, uint32_t, uint16_t, uint16_t, uint32_t);
typedef void* CVoidPtr;

void push_positions(lua_State L, Position* positions, int32_t len, uint32_t cap);
// #define push_positions(L, positions, len, cap)\
// {\
//     lua_createtable(L, 0, 0);\
//     lua_pushnumber(L, -1);\
//     lua_rawseti(L, -2, 0);\
// 	if (positions){\
//         for (size_t i = 0; i < len; i++)\
//         {\
//             const auto& p = positions[i];\
//             lua_createtable(L, 0, 0);\
//             lua_pushstring(L, "x");\
//             lua_pushinteger(L, p.x);\
//             lua_rawseti(L, -2, 1);\
//             lua_pushstring(L, "y");\
//             lua_pushinteger(L, p.y);\
//             lua_rawseti(L, -2, 2);\
//             lua_rawseti(L, -2, i + 1);\
//         }\
// 		ReleaseVector((unsigned char*)positions, len, sizeof(Position), cap);\
// 	}\
// }
#define push_integers(L, ptr, len, tp, cap)\
{\
	lua_createtable(L, 0, 0); \
	if (len >= 0) {\
        tp* array = (tp*)ptr;\
        for (size_t i = 0; i < len; i++)\
        {\
            lua_pushnumber(L, (double)array[i]); \
            lua_rawseti(L, -2, i + 1); \
        }\
		ReleaseVector((unsigned char*)ptr, len, sizeof(tp), cap);\
	}\
}
#define to_array(L, pos, tp, out)\
tp* out = nullptr;\
{\
    int size = tolua_objlen(L, pos);\
    auto top = lua_gettop(L);\
    out = (tp*)malloc(size * sizeof(tp));\
    top ++;\
    for (size_t i = 1; i <= size; i++)\
    {\
		lua_rawgeti(L, pos, i);\
		out[i - 1] = lua_tointeger(L, top);\
		lua_pop(L, 1);\
    }\
    lua_settop(L, pos + 1);\
}