#pragma once
#include "tolua.h"
#include <stack>
class LuaFunction
{
	struct FuncData
	{
		int32_t oldTop;
		int32_t statckPos;
	};
public:
	LuaFunction() = delete;
	LuaFunction(lua_State L);
	~LuaFunction();
	bool Load(const char* functionPath);
	int32_t BeginCall();
	bool PCall();
	void EndCall();
	void Push(int32_t integer);
	void Push(const char* buffer, int32_t size);
private:
	int32_t reference;
	int32_t oldTop;
	int32_t argCount;
	int32_t stackPos;
	std::stack<FuncData> stack;
	lua_State lua;
};
