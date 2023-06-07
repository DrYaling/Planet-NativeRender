/*
 * created by zxb
 * lua 接口
 */
#include "tolua.h"
#include <vector>
#include <string>
#include <map>
#include "../Logger/Logger.h"
#include "../core_api.h"
#include "LuaFunction.h"
#include "RenderingPlugin.h"
static int luaFunctionCount = 0;
void push_positions(lua_State L, Position* positions, int32_t len, uint32_t cap)
{
	auto top = lua_gettop(L);
	lua_createtable(L, 0, 0);
	//LogFormat("push positions top %d, cur top %d, len %d, cap %d", top, lua_gettop(L), len, cap);
	if (len >= 0) {
		for (size_t i = 0; i < len; i++)
		{
			const auto& p = positions[i]; 
			lua_createtable(L, 0, 0); 
			lua_pushnumber(L, p.x);
			lua_rawseti(L, -2, 1);
			lua_pushnumber(L, p.y);
			lua_rawseti(L, -2, 2); 
			lua_rawseti(L, -2, i + 1); 
		}
		ReleaseVector((unsigned char*)positions, len, sizeof(Position), cap); 
	}
	//LogFormat("push positions lua_rawseti top %d, cur top %d", top, lua_gettop(L));
	//lua_rawseti(L, -2, lua_gettop(L) + 1); 
}
bool CheckLuaTypeValid(lua_State L, int32_t pos, int32_t expected)
{
	const auto lt = lua_type(L, pos);
	if (lt != expected)
	{
		LogErrorFormat("get parameter %d error, expected %d, got %d", pos, expected, lt);
		return false;
	}
	return true;
}
bool BeginClass(lua_State L, const char* className)
{
	//LogFormat("begin class %s", className == nullptr ? "_G" : className);
	//if(nullClass != 0)
	//{
	//	LogError("reg class %s while class %d reg was not ended", className == nullptr? "nullptr" : className, regClass.c_str());
	//	return false;
	//}
	if (tolua_beginmodule(L, className))
	{
		return true;
	}
	lua_settop(L, 0);
	return false;
}
void EndClass(lua_State L, const char* className)
{
	//LogFormat("end class %s", className == nullptr ? "_G" : className);
	tolua_endmodule(L);
}
void RegisterFunc(lua_State L, const char* className, const char* fnName, lua_CFunction fn)
{
	const char* check_name = className == nullptr ? "" : className;
	//LogFormat("reg func %s: %s", check_name, fnName);
	tolua_function(L, fnName, fn);
	luaFunctionCount++;
}
void InitLuaState()
{
}
/*
 * 网络模块
 */
///lua虚拟机
LuaFunction* event_handler = nullptr;
const char* empty_buffer = {};
void LuaNetEventCallback(int32_t error_code, uint32_t session_id, const char* msg_buffer, uint32_t msg_size, uint16_t opcode, uint16_t sub_code, uint32_t rpc)
{
	if(nullptr != event_handler)
	{
		event_handler->BeginCall();
		event_handler->Push(error_code);
		event_handler->Push(session_id);
		event_handler->Push(msg_buffer, msg_size);
		event_handler->Push(msg_size);
		event_handler->Push(opcode);
		event_handler->Push(sub_code);
		event_handler->Push(rpc);
		event_handler->PCall();
		event_handler->EndCall();
	}
	else
	{
		LogError("event_handler empty");
	}
}
///初始化网络工作线程
int LuaNetInitWorker(lua_State L)
{
	CheckArgsCount(L, 1)
	CheckLuaType(L, 1, LUA_TNUMBER)
	const uint32_t worker_count = lua_tointeger(L, 1);
	lua_pushboolean(L, NetInitWorker(worker_count) ? 1 : 0);
	return 1;
}
int LuaNetCreateSession(lua_State L)
{
	CheckArgsCount(L, 3)
	CheckLuaType(L, 1, LUA_TSTRING)
	const auto host = lua_tolstring(L, 1, nullptr);
	CheckLuaType(L, 2, LUA_TNUMBER)
	const auto port = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER)
	const auto st = lua_tointeger(L, 3);
	if(port<=0 || port >= 65536)
	{
		LogErrorFormat("expected port from 1~65535, got %d", port);
		return 1;
	}
	const auto success = NetCreateSession(host, port, st, &LuaNetEventCallback);
	lua_pushboolean(L, success ? 1 : 0);
	return 1;
}
int LuaNetSendMsg(lua_State L)
{
	CheckArgsCount(L, 6)
	CheckLuaType(L, 1, LUA_TNUMBER)
	const auto session_id = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TSTRING)
	const auto msg = lua_tolstring(L, 2, nullptr);
	CheckLuaType(L, 3, LUA_TNUMBER)
	const auto msg_len = lua_tointeger(L, 3);
	CheckLuaType(L, 4, LUA_TNUMBER)
	const auto opcode = lua_tointeger(L, 4);
	CheckLuaType(L, 5, LUA_TNUMBER)
	const auto sub_code = lua_tointeger(L, 5);
	CheckLuaType(L, 6, LUA_TNUMBER)
	const auto rpc = lua_tointeger(L, 6);
	lua_pushboolean(L, NetSendMsg(session_id, reinterpret_cast<const unsigned char*>(msg), msg_len, opcode, sub_code, rpc) ? 1 : 0);
	return 1;
}
int LuaNetCloseSession(lua_State L)
{
	CheckArgsCount(L, 1)
	CheckLuaType(L, 1, LUA_TNUMBER)
	const auto session = lua_tointeger(L, 1);
	NetCloseSession(session);
	return 1;
}
int LuaNetCloseNetwork(lua_State L)
{
	CheckArgsCount(L, 0)
	NetCloseNetwork();
	return 1;
}
void CloseLuaEngine()
{
	luaFunctionCount = 0;
	safe_delete(event_handler)
}
///设置日志基本别
int SetLogLevel(lua_State L)
{
	CheckArgsCount(L, 1)
	CheckLuaType(L, 1, LUA_TNUMBER)
	const uint32_t level = lua_tointeger(L, 1);
	CaSetLogLevel(level);
	return 1;
}
int LoadLuaFile(lua_State L)
{
	const char* file_name = lua_tolstring(L, 1, nullptr);
	if (!file_name)
	{
		LogError("load lua file with no name");
		return 1;
	}
	const char* buffer = nullptr;
	int32_t buffer_size = 0;
	uint32_t cap_size = 0;
	const int32_t result = LoadFile(file_name, &buffer, &buffer_size, &cap_size);
	if(result != 0)
	{
		LogErrorFormat("load lua file %s fail with err %d", file_name, result);
		lua_pushstring(L, "fail to load file");
		return 1;
	}
	std::string fn = "@";
	fn.append(file_name);
	if (tolua_loadbuffer(L, buffer, buffer_size, fn.c_str()) != 0)
	{
		LogErrorFormat("tolua_loadbuffer fail with file %d", fn.c_str());
		lua_pushstring(L, "fail to load byte buffer");
	}
	ReleaseVector((unsigned char*)(buffer), buffer_size, sizeof(char), cap_size);
	return 1;
}
void AddLuaLoader(lua_State L)
{
	if (tolua_getfield(L, LUA_GLOBALSINDEX, "package") != 0) {
		LogErrorFormat("AddLuaLoader package fail %s", lua_tolstring(L, -1, nullptr));
		return;
	}
	if (tolua_getfield(L, -1, "loaders") != 0) {
		LogErrorFormat("AddLuaLoader loaders fail %s", lua_tolstring(L, -1, nullptr));
		return;
	}
	tolua_pushcfunction(L, &LoadLuaFile);
	luaFunctionCount ++;
	for (int i = tolua_objlen(L, -2) + 1; i > 2; i--)
	{
		lua_rawgeti(L, -2, i - 1);
		lua_rawseti(L, -3, i);
	}

	lua_rawseti(L, -2, 2);
	lua_pop(L, 2);	
}
void LuaInitBinders(lua_State L, bool debugMode)
{
	auto const start_time = TimestampMills();
	BeginClass(L, nullptr);
	RegisterFunc(L, nullptr, "CaLuaNetInitWorker", &LuaNetInitWorker);
	RegisterFunc(L, nullptr, "CaLuaNetSendMsg", &LuaNetSendMsg);
	RegisterFunc(L, nullptr, "CaLuaNetCreateSession", &LuaNetCreateSession);
	RegisterFunc(L, nullptr, "CaLuaNetCloseSession", &LuaNetCloseSession);
	RegisterFunc(L, nullptr, "CaLuaNetCloseNetwork", &LuaNetCloseNetwork);
	RegisterFunc(L, nullptr, "CaSetLogLevel", &SetLogLevel);
	EndClass(L, nullptr);
	InitRust2Lua(L);
	//非调试模式才会初始化lua加载器
	if(!debugMode)
		AddLuaLoader(L);
	auto const end_time = TimestampMills();
	LogFormat("LuaInitBinders %d function binded, cost %d ms", luaFunctionCount, end_time - start_time);
}
void LuaLateInit(lua_State L)
{
	event_handler = new LuaFunction(L);
	Log("LuaLateInit");
	if (!event_handler->Load("LuaNetEventCallback"))
	{
		safe_delete(event_handler);
		LogError("fail to load LuaNetEventCallback");
	}
}