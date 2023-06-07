/*
auto generated c header, do not modify this
*/
#include "../core_api.h"
#include "tolua.h"
int CaFtCreateBattle(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TSTRING);
	const auto buffer = lua_tolstring(L, 1, nullptr);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto len = lua_tointeger(L, 2);
	const auto call_result = FtCreateBattle((unsigned const char*)buffer, (uint32_t)len);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaFtDestroyBattle(lua_State L){
	CheckArgsCount(L, 0);
	const auto call_result = FtDestroyBattle();
	lua_pushboolean(L, call_result);
	return 1;
}
int CaFtLoadConfig(lua_State L){
	CheckArgsCount(L, 3);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto config_type = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TSTRING);
	const auto content = lua_tolstring(L, 2, nullptr);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto len = lua_tointeger(L, 3);
	const auto call_result = FtLoadConfig((int32_t)config_type, (unsigned const char*)content, (uint32_t)len);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaFtUnloadAllConfigs(lua_State L){
	CheckArgsCount(L, 0);
	FtUnloadAllConfigs();
	return 1;
}
int CaFtPointFree(lua_State L){
	CheckArgsCount(L, 3);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto map = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto x = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto y = lua_tointeger(L, 3);
	const auto call_result = FtPointFree((uint32_t)map, (int32_t)x, (int32_t)y);
	lua_pushboolean(L, call_result);
	return 1;
}
int CaFtEnterBattle(lua_State L){
	CheckArgsCount(L, 1);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto team = lua_tointeger(L, 1);
	const auto call_result = FtEnterBattle((uint32_t)team);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaFtStartBattle(lua_State L){
	CheckArgsCount(L, 1);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto team = lua_tointeger(L, 1);
	unsigned const char* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtStartBattle((uint32_t)team, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	if(nullptr != out_buffer){
		lua_pushlstring(L, (const char*)out_buffer, out_len);
		ReleaseVector((unsigned char*)out_buffer, out_len, sizeof(const char), out_cap);
	}
	else{
		lua_pushnil(L);
	}
	return 2;
}
int CaFtPrepareBattle(lua_State L){
	CheckArgsCount(L, 3);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto team = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TSTRING);
	const auto buffer = lua_tolstring(L, 2, nullptr);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto len = lua_tointeger(L, 3);
	unsigned const char* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtPrepareBattle((uint32_t)team, (unsigned const char*)buffer, (uint32_t)len, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	if(nullptr != out_buffer){
		lua_pushlstring(L, (const char*)out_buffer, out_len);
		ReleaseVector((unsigned char*)out_buffer, out_len, sizeof(const char), out_cap);
	}
	else{
		lua_pushnil(L);
	}
	return 2;
}
int CaFtMoveTo(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TSTRING);
	const auto buffer = lua_tolstring(L, 1, nullptr);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto len = lua_tointeger(L, 2);
	unsigned const char* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtMoveTo((unsigned const char*)buffer, (uint32_t)len, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	if(nullptr != out_buffer){
		lua_pushlstring(L, (const char*)out_buffer, out_len);
		ReleaseVector((unsigned char*)out_buffer, out_len, sizeof(const char), out_cap);
	}
	else{
		lua_pushnil(L);
	}
	return 2;
}
int CaFtGetPathToPoint(lua_State L){
	CheckArgsCount(L, 3);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto uuid = tolua_toint64(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto target_x = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto target_y = lua_tointeger(L, 3);
	Position* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtGetPathToPoint((uint64_t)uuid, (int32_t)target_x, (int32_t)target_y, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	push_positions(L, out_buffer, out_len, out_cap);
	return 2;
}
int CaFtGetPathToEnemy(lua_State L){
	CheckArgsCount(L, 5);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto uuid = tolua_toint64(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto target_id = tolua_toint64(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto skill_id = lua_tointeger(L, 3);
	CheckLuaType(L, 4, LUA_TNUMBER);
	const auto v_x = lua_tointeger(L, 4);
	CheckLuaType(L, 5, LUA_TNUMBER);
	const auto v_y = lua_tointeger(L, 5);
	Position* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtGetPathToEnemy((uint64_t)uuid, (uint64_t)target_id, (uint32_t)skill_id, (int32_t)v_x, (int32_t)v_y, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	push_positions(L, out_buffer, out_len, out_cap);
	return 2;
}
int CaFtGetPathToSkillTarget(lua_State L){
	CheckArgsCount(L, 6);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto uuid = tolua_toint64(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto skill_id = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto target_x = lua_tointeger(L, 3);
	CheckLuaType(L, 4, LUA_TNUMBER);
	const auto target_y = lua_tointeger(L, 4);
	CheckLuaType(L, 5, LUA_TNUMBER);
	const auto v_x = lua_tointeger(L, 5);
	CheckLuaType(L, 6, LUA_TNUMBER);
	const auto v_y = lua_tointeger(L, 6);
	Position* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtGetPathToSkillTarget((uint64_t)uuid, (uint32_t)skill_id, (int32_t)target_x, (int32_t)target_y, (int32_t)v_x, (int32_t)v_y, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	push_positions(L, out_buffer, out_len, out_cap);
	return 2;
}
int CaFtGetTargetDistance(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto uuid = tolua_toint64(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto target_id = tolua_toint64(L, 2);
	const auto call_result = FtGetTargetDistance((uint64_t)uuid, (uint64_t)target_id);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaFtGetDistanceTo(lua_State L){
	CheckArgsCount(L, 4);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto uuid = tolua_toint64(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto target_x = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto target_y = lua_tointeger(L, 3);
	CheckLuaType(L, 4, LUA_TBOOLEAN);
	const auto path_distance = tolua_toboolean(L, 4);
	const auto call_result = FtGetDistanceTo((uint64_t)uuid, (int32_t)target_x, (int32_t)target_y, (bool)path_distance);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaFtBattleAiUpdate(lua_State L){
	CheckArgsCount(L, 0);
	const auto call_result = FtBattleAiUpdate();
	lua_pushboolean(L, call_result);
	return 1;
}
int CaFtCastSkill(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TSTRING);
	const auto buffer = lua_tolstring(L, 1, nullptr);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto len = lua_tointeger(L, 2);
	unsigned const char* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtCastSkill((unsigned const char*)buffer, (uint32_t)len, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	if(nullptr != out_buffer){
		lua_pushlstring(L, (const char*)out_buffer, out_len);
		ReleaseVector((unsigned char*)out_buffer, out_len, sizeof(const char), out_cap);
	}
	else{
		lua_pushnil(L);
	}
	return 2;
}
int CaFtBattleUpdateTurn(lua_State L){
	CheckArgsCount(L, 0);
	FtBattleUpdateTurn();
	return 1;
}
int CaFtTakeRecords(lua_State L){
	CheckArgsCount(L, 0);
	unsigned const char* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtTakeRecords(&out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	if(nullptr != out_buffer){
		lua_pushlstring(L, (const char*)out_buffer, out_len);
		ReleaseVector((unsigned char*)out_buffer, out_len, sizeof(const char), out_cap);
	}
	else{
		lua_pushnil(L);
	}
	return 2;
}
int CaFtGetRoleAttribute(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto uuid = tolua_toint64(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto attr = lua_tointeger(L, 2);
	const auto call_result = FtGetRoleAttribute((uint64_t)uuid, (int32_t)attr);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaFtSkillPredict(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TSTRING);
	const auto buffer = lua_tolstring(L, 1, nullptr);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto len = lua_tointeger(L, 2);
	unsigned const char* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtSkillPredict((unsigned const char*)buffer, (uint32_t)len, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	if(nullptr != out_buffer){
		lua_pushlstring(L, (const char*)out_buffer, out_len);
		ReleaseVector((unsigned char*)out_buffer, out_len, sizeof(const char), out_cap);
	}
	else{
		lua_pushnil(L);
	}
	return 2;
}
int CaFtReqEvent(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TSTRING);
	const auto buffer = lua_tolstring(L, 1, nullptr);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto len = lua_tointeger(L, 2);
	unsigned const char* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtReqEvent((unsigned const char*)buffer, (uint32_t)len, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	if(nullptr != out_buffer){
		lua_pushlstring(L, (const char*)out_buffer, out_len);
		ReleaseVector((unsigned char*)out_buffer, out_len, sizeof(const char), out_cap);
	}
	else{
		lua_pushnil(L);
	}
	return 2;
}
int CaFtCheckSkillValid(lua_State L){
	CheckArgsCount(L, 5);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto uuid = tolua_toint64(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto skill_id = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto cast_pos = lua_tointeger(L, 3);
	CheckLuaType(L, 4, LUA_TNUMBER);
	const auto target_id = tolua_toint64(L, 4);
	CheckLuaType(L, 5, LUA_TBOOLEAN);
	const auto point_target = tolua_toboolean(L, 5);
	const auto call_result = FtCheckSkillValid((uint64_t)uuid, (uint32_t)skill_id, (uint16_t)cast_pos, (uint64_t)target_id, (bool)point_target);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaFtStatusSynchronize(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TSTRING);
	const auto buffer = lua_tolstring(L, 1, nullptr);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto len = lua_tointeger(L, 2);
	const auto call_result = FtStatusSynchronize((unsigned const char*)buffer, (uint32_t)len);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaFtGetValidTargets(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto uuid = tolua_toint64(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto skill_id = lua_tointeger(L, 2);
	const uint64_t* targets = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtGetValidTargets((uint64_t)uuid, (uint32_t)skill_id, &targets, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	push_integers(L, targets, out_len, const uint64_t, out_cap);
	return 2;
}
int CaFtGetAttackers(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto uuid = tolua_toint64(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto point_id = lua_tointeger(L, 2);
	const uint64_t* targets = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = FtGetAttackers((uint64_t)uuid, (uint32_t)point_id, &targets, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	push_integers(L, targets, out_len, const uint64_t, out_cap);
	return 2;
}
int CaSetRustLogLevel(lua_State L){
	CheckArgsCount(L, 1);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto log_level = lua_tointeger(L, 1);
	const auto call_result = SetRustLogLevel((int32_t)log_level);
	lua_pushboolean(L, call_result);
	return 1;
}
int CaMainUpdate(lua_State L){
	CheckArgsCount(L, 0);
	MainUpdate();
	return 1;
}
int CaRsOnApplicationQuit(lua_State L){
	CheckArgsCount(L, 0);
	RsOnApplicationQuit();
	return 1;
}
int CaTimestampMills(lua_State L){
	CheckArgsCount(L, 0);
	const auto call_result = TimestampMills();
	tolua_pushint64(L, call_result);
	return 1;
}
int CaReleaseVector(lua_State L){
	CheckArgsCount(L, 4);
	CheckLuaType(L, 1, LUA_TTABLE);
	to_array(L, 1, unsigned char, ptr);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto size = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto type_size = lua_tointeger(L, 3);
	CheckLuaType(L, 4, LUA_TNUMBER);
	const auto cap = lua_tointeger(L, 4);
	ReleaseVector((unsigned char*)ptr, (uint32_t)size, (uint32_t)type_size, (uint32_t)cap);
	if (ptr) delete[] ptr;
	return 1;
}
int CaExpInverseDirection(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto dx = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto dy = lua_tointeger(L, 2);
	int32_t x;
	int32_t y;
	ExpInverseDirection((int32_t)dx, (int32_t)dy, &x, &y);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 2;
}
int CaIsBaseDir(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto x = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto y = lua_tointeger(L, 2);
	const auto call_result = IsBaseDir((int32_t)x, (int32_t)y);
	lua_pushboolean(L, call_result);
	return 1;
}
int CaGetHexId(lua_State L){
	CheckArgsCount(L, 3);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto x = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto y = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto map = lua_tointeger(L, 3);
	const auto call_result = GetHexId((int32_t)x, (int32_t)y, (uint32_t)map);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaGetHexPosition(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto id = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto map = lua_tointeger(L, 2);
	int32_t x;
	int32_t y;
	GetHexPosition((uint32_t)id, (uint32_t)map, &x, &y);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 2;
}
int CaGetMapSize(lua_State L){
	CheckArgsCount(L, 1);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto map = lua_tointeger(L, 1);
	const auto call_result = GetMapSize((uint32_t)map);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaExpDestroyMap(lua_State L){
	CheckArgsCount(L, 0);
	ExpDestroyMap();
	return 1;
}
int CaExpCreateMap(lua_State L){
	CheckArgsCount(L, 4);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto width = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto height = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TTABLE);
	to_array(L, 3, uint32_t, barrier_ptr);
	CheckLuaType(L, 4, LUA_TNUMBER);
	const auto barrier_count = lua_tointeger(L, 4);
	const auto call_result = ExpCreateMap((uint32_t)width, (uint32_t)height, (uint32_t*)barrier_ptr, (uint32_t)barrier_count);
	lua_pushboolean(L, call_result);
	if (barrier_ptr) delete[] barrier_ptr;
	return 1;
}
int CaExpFindPath(lua_State L){
	CheckArgsCount(L, 5);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto start_x = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto start_y = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto target_x = lua_tointeger(L, 3);
	CheckLuaType(L, 4, LUA_TNUMBER);
	const auto target_y = lua_tointeger(L, 4);
	CheckLuaType(L, 5, LUA_TNUMBER);
	const auto distance = lua_tointeger(L, 5);
	Position* out_buffer = nullptr;
	int32_t out_len;
	uint32_t out_cap;
	const auto call_result = ExpFindPath((int32_t)start_x, (int32_t)start_y, (int32_t)target_x, (int32_t)target_y, (uint32_t)distance, &out_buffer, &out_len, &out_cap);
	lua_pushnumber(L, call_result);
	push_positions(L, out_buffer, out_len, out_cap);
	return 2;
}
int CaExpUpdateGridState(lua_State L){
	CheckArgsCount(L, 3);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto grid = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TBOOLEAN);
	const auto block = tolua_toboolean(L, 2);
	CheckLuaType(L, 3, LUA_TBOOLEAN);
	const auto explore_map = tolua_toboolean(L, 3);
	ExpUpdateGridState((uint32_t)grid, (bool)block, (bool)explore_map);
	return 1;
}
int CaExpGetDistance(lua_State L){
	CheckArgsCount(L, 5);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto x = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto y = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto target_x = lua_tointeger(L, 3);
	CheckLuaType(L, 4, LUA_TNUMBER);
	const auto target_y = lua_tointeger(L, 4);
	CheckLuaType(L, 5, LUA_TBOOLEAN);
	const auto path_distance = tolua_toboolean(L, 5);
	const auto call_result = ExpGetDistance((int32_t)x, (int32_t)y, (int32_t)target_x, (int32_t)target_y, (bool)path_distance);
	lua_pushnumber(L, call_result);
	return 1;
}
int CaExpRemoveFogs(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TTABLE);
	to_array(L, 1, uint32_t, list);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto list_count = lua_tointeger(L, 2);
	ExpRemoveFogs((uint32_t*)list, (uint32_t)list_count);
	if (list) delete[] list;
	return 1;
}
int CaExpAddFogs(lua_State L){
	CheckArgsCount(L, 2);
	CheckLuaType(L, 1, LUA_TTABLE);
	to_array(L, 1, uint32_t, list);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto list_count = lua_tointeger(L, 2);
	ExpAddFogs((uint32_t*)list, (uint32_t)list_count);
	if (list) delete[] list;
	return 1;
}
int CaExpAddAllFogsExceptDistance(lua_State L){
	CheckArgsCount(L, 3);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto x = lua_tointeger(L, 1);
	CheckLuaType(L, 2, LUA_TNUMBER);
	const auto y = lua_tointeger(L, 2);
	CheckLuaType(L, 3, LUA_TNUMBER);
	const auto distance = lua_tointeger(L, 3);
	ExpAddAllFogsExceptDistance((int32_t)x, (int32_t)y, (uint32_t)distance);
	return 1;
}
int CaExpClearAllFogs(lua_State L){
	CheckArgsCount(L, 0);
	ExpClearAllFogs();
	return 1;
}
int CaGetSessionState(lua_State L){
	CheckArgsCount(L, 1);
	CheckLuaType(L, 1, LUA_TNUMBER);
	const auto session_id = lua_tointeger(L, 1);
	const auto call_result = GetSessionState((uint32_t)session_id);
	lua_pushnumber(L, call_result);
	return 1;
}
void InitRust2Lua(lua_State L){
  BeginClass(L, nullptr);
  RegisterFunc(L, nullptr, "CaFtCreateBattle", &CaFtCreateBattle);
  RegisterFunc(L, nullptr, "CaFtDestroyBattle", &CaFtDestroyBattle);
  RegisterFunc(L, nullptr, "CaFtLoadConfig", &CaFtLoadConfig);
  RegisterFunc(L, nullptr, "CaFtUnloadAllConfigs", &CaFtUnloadAllConfigs);
  RegisterFunc(L, nullptr, "CaFtPointFree", &CaFtPointFree);
  RegisterFunc(L, nullptr, "CaFtEnterBattle", &CaFtEnterBattle);
  RegisterFunc(L, nullptr, "CaFtStartBattle", &CaFtStartBattle);
  RegisterFunc(L, nullptr, "CaFtPrepareBattle", &CaFtPrepareBattle);
  RegisterFunc(L, nullptr, "CaFtMoveTo", &CaFtMoveTo);
  RegisterFunc(L, nullptr, "CaFtGetPathToPoint", &CaFtGetPathToPoint);
  RegisterFunc(L, nullptr, "CaFtGetPathToEnemy", &CaFtGetPathToEnemy);
  RegisterFunc(L, nullptr, "CaFtGetPathToSkillTarget", &CaFtGetPathToSkillTarget);
  RegisterFunc(L, nullptr, "CaFtGetTargetDistance", &CaFtGetTargetDistance);
  RegisterFunc(L, nullptr, "CaFtGetDistanceTo", &CaFtGetDistanceTo);
  RegisterFunc(L, nullptr, "CaFtBattleAiUpdate", &CaFtBattleAiUpdate);
  RegisterFunc(L, nullptr, "CaFtCastSkill", &CaFtCastSkill);
  RegisterFunc(L, nullptr, "CaFtBattleUpdateTurn", &CaFtBattleUpdateTurn);
  RegisterFunc(L, nullptr, "CaFtTakeRecords", &CaFtTakeRecords);
  RegisterFunc(L, nullptr, "CaFtGetRoleAttribute", &CaFtGetRoleAttribute);
  RegisterFunc(L, nullptr, "CaFtSkillPredict", &CaFtSkillPredict);
  RegisterFunc(L, nullptr, "CaFtReqEvent", &CaFtReqEvent);
  RegisterFunc(L, nullptr, "CaFtCheckSkillValid", &CaFtCheckSkillValid);
  RegisterFunc(L, nullptr, "CaFtStatusSynchronize", &CaFtStatusSynchronize);
  RegisterFunc(L, nullptr, "CaFtGetValidTargets", &CaFtGetValidTargets);
  RegisterFunc(L, nullptr, "CaFtGetAttackers", &CaFtGetAttackers);
  RegisterFunc(L, nullptr, "CaSetRustLogLevel", &CaSetRustLogLevel);
  RegisterFunc(L, nullptr, "CaMainUpdate", &CaMainUpdate);
  RegisterFunc(L, nullptr, "CaRsOnApplicationQuit", &CaRsOnApplicationQuit);
  RegisterFunc(L, nullptr, "CaTimestampMills", &CaTimestampMills);
  RegisterFunc(L, nullptr, "CaReleaseVector", &CaReleaseVector);
  RegisterFunc(L, nullptr, "CaExpInverseDirection", &CaExpInverseDirection);
  RegisterFunc(L, nullptr, "CaIsBaseDir", &CaIsBaseDir);
  RegisterFunc(L, nullptr, "CaGetHexId", &CaGetHexId);
  RegisterFunc(L, nullptr, "CaGetHexPosition", &CaGetHexPosition);
  RegisterFunc(L, nullptr, "CaGetMapSize", &CaGetMapSize);
  RegisterFunc(L, nullptr, "CaExpDestroyMap", &CaExpDestroyMap);
  RegisterFunc(L, nullptr, "CaExpCreateMap", &CaExpCreateMap);
  RegisterFunc(L, nullptr, "CaExpFindPath", &CaExpFindPath);
  RegisterFunc(L, nullptr, "CaExpUpdateGridState", &CaExpUpdateGridState);
  RegisterFunc(L, nullptr, "CaExpGetDistance", &CaExpGetDistance);
  RegisterFunc(L, nullptr, "CaExpRemoveFogs", &CaExpRemoveFogs);
  RegisterFunc(L, nullptr, "CaExpAddFogs", &CaExpAddFogs);
  RegisterFunc(L, nullptr, "CaExpAddAllFogsExceptDistance", &CaExpAddAllFogsExceptDistance);
  RegisterFunc(L, nullptr, "CaExpClearAllFogs", &CaExpClearAllFogs);
  RegisterFunc(L, nullptr, "CaGetSessionState", &CaGetSessionState);
  EndClass(L, nullptr);
}