#pragma once
#include "../define.h"
#ifndef DLL_IMPORT
#define DLL_IMPORT //__declspec(dllimport)
#endif
#if WIN32
#pragma comment(lib,"tolua")
#endif
EXTERN_C_BEGIN
enum LuaTypes
{
    LUA_TNONE = -1,
    LUA_TNIL = 0,
    LUA_TBOOLEAN = 1,
    LUA_TLIGHTUSERDATA = 2,
    LUA_TNUMBER = 3,
    LUA_TSTRING = 4,
    LUA_TTABLE = 5,
    LUA_TFUNCTION = 6,
    LUA_TUSERDATA = 7,
    LUA_TTHREAD = 8,

};
static int32_t LUA_REGISTRYINDEX = -10000;
static int32_t LUA_ENVIRONINDEX = -10001;
static int32_t LUA_GLOBALSINDEX = -10002;
typedef void* lua_State;
typedef int (*lua_CFunction) (lua_State L);
DLL_IMPORT int32_t lua_gettop(lua_State luaState);
DLL_IMPORT void lua_settop(lua_State luaState, int top);
DLL_IMPORT int32_t lua_tointeger(lua_State luaState, int idx);
DLL_IMPORT double lua_tonumber(lua_State luaState, int idx);
//DLL_IMPORT void* lua_tolightuserdata(lua_State luaState, int idx);
//DLL_IMPORT void* lua_touserdata(lua_State luaState, int idx);
DLL_IMPORT void tolua_pushint64(lua_State luaState, int64_t n);
DLL_IMPORT int64_t tolua_toint64(lua_State luaState, int idx);
DLL_IMPORT const char* lua_tolstring(lua_State luaState, int index, size_t* len);
DLL_IMPORT void lua_pushinteger(lua_State luaState, int32_t integer);
//DLL_IMPORT void lua_pushlightuserdata(lua_State luaState, void* udata);
DLL_IMPORT void lua_pushnumber(lua_State luaState, double number);
DLL_IMPORT void lua_pushnil(lua_State luaState);
DLL_IMPORT int32_t toluaL_ref(lua_State luaState);
DLL_IMPORT void lua_pushstring(lua_State luaState, const char* str);
DLL_IMPORT void tolua_pushlstring(lua_State luaState, const char* str, int size);
DLL_IMPORT void lua_pushlstring(lua_State luaState, const char* str, size_t size);
bool DLL_IMPORT tolua_beginmodule(lua_State L, const char* name);
void DLL_IMPORT tolua_endmodule(lua_State L);
int DLL_IMPORT tolua_beginclass(lua_State L, const char* name, int meta_ref, int reference);
void DLL_IMPORT tolua_endclass(lua_State L);
void DLL_IMPORT tolua_function(lua_State L, const char* name, lua_CFunction fn);
void DLL_IMPORT tolua_pushcfunction(lua_State L, lua_CFunction fn);
void DLL_IMPORT lua_getfield(lua_State L, int, const char* name);
int DLL_IMPORT lua_pcall(lua_State L, int, int, int);
int DLL_IMPORT lua_call(lua_State L, int, int);
DLL_IMPORT int32_t lua_type(lua_State luaState, int idx);
int DLL_IMPORT tolua_beginpcall(lua_State L, int reference);
DLL_IMPORT void toluaL_unref(lua_State L, int reference);
bool DLL_IMPORT tolua_toboolean(lua_State L, int idx);
DLL_IMPORT void lua_pushboolean(lua_State luaState, int value);
DLL_IMPORT int32_t tolua_rawnetobj(lua_State luaState, int obj);
void DLL_IMPORT tolua_variable(lua_State L, const char* name, lua_CFunction get, lua_CFunction set);

DLL_IMPORT void lua_createtable(lua_State L, int narr, int nrec);
DLL_IMPORT void lua_rawseti(lua_State L, int tableIndex, int idx);
DLL_IMPORT void tolua_setfield(lua_State L, int idx, const char* key);
DLL_IMPORT int tolua_gettable(lua_State L,int idx);
DLL_IMPORT int tolua_objlen(lua_State L, int idx);
DLL_IMPORT void lua_rawgeti(lua_State L, int idx, int n);
DLL_IMPORT int tolua_getfield(lua_State L, int idx, const char* key);
DLL_IMPORT int tolua_loadbuffer(lua_State L, const char* buffer, int size, const char* buffer_name);

void RegisterFunc(lua_State L, const char* className, const char* fnName, lua_CFunction fn);
void EndClass(lua_State L, const char* className);
bool BeginClass(lua_State L, const char* className);
/// <summary>
/// 初始化Lua引擎
/// </summary>
void InitLuaState();
///初始化lua绑定数据
void LuaInitBinders(lua_State L, bool debugMode);
void LuaLateInit(lua_State L);
void CloseLuaEngine();
///初始化rust接口
void InitRust2Lua(lua_State L);
bool CheckLuaTypeValid(lua_State L, int32_t pos, int32_t expected);
EXTERN_C_END


#define CheckArgsCount(L, C) {\
	const auto c = lua_gettop(L);\
	if(c != C) {\
		LogErrorFormat("CheckArgsCount fail, expected %d, got %d", C, c);\
		return 1;\
	}\
}
#define CheckLuaType(L, P, E) {\
	if (!CheckLuaTypeValid(L, P, E))\
	{\
		return 1;\
	}\
}
#define lua_pop(L,n)		lua_settop(L, -(n)-1)