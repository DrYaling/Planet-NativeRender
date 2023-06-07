/*
 * created by zxb
 * lua 全局函数
 */
#include "LuaFunction.h"

#include "../Logger/Logger.h"

LuaFunction::LuaFunction(lua_State lua):
oldTop(-1),
stackPos(-1),
lua(lua)
{
}

LuaFunction::~LuaFunction()
{
	//toluaL_unref(lua, reference);
	lua = nullptr;
}

bool LuaFunction::Load(const char* functionPath)
{
	const auto oldTop = lua_gettop(lua);
	lua_getfield(lua, LUA_GLOBALSINDEX, functionPath);
	auto type = lua_type(lua, -1);
	if(type != LUA_TFUNCTION)
	{
		lua_settop(lua, oldTop);
		return false;
	}
	this->reference = toluaL_ref(lua);
	return true;
}

int32_t LuaFunction::BeginCall()
{
	stack.push(FuncData{
		oldTop, stackPos
	});
	oldTop = tolua_beginpcall(lua, reference);
	argCount = 0;
	stackPos = -1;
	return oldTop;
}

bool LuaFunction::PCall()
{
	stackPos = oldTop + 1;
	const auto pr = lua_pcall(lua, argCount, -1, oldTop);
	if(pr != 0)
	{
		LogErrorFormat("fall to call %s", lua_tolstring(lua, -1, nullptr));
		return false;
	}
	return true;
}

void LuaFunction::EndCall()
{
	lua_settop(lua, oldTop - 1);
	argCount = 0;
	const auto data = stack.top();
	stack.pop();
	oldTop = data.oldTop;
	stackPos = data.statckPos;	
}

void LuaFunction::Push(int32_t integer)
{
	if (integer >= 0)
		lua_pushinteger(lua, integer);
	else
		lua_pushnumber(lua, integer);
	++argCount;
}

void LuaFunction::Push(const char* buffer, int32_t size)
{
	if(buffer == nullptr)
		lua_pushnil(lua);
	else
		lua_pushlstring(lua, buffer, size);
	++argCount;
}
