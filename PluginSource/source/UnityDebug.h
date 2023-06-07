#pragma once

#include"string.h"
#include "stdio.h"
#include <stdarg.h>
#include "Unity/IUnityInterface.h"
#define UNITY_LOG(acStr, ...)  Debug::L(acStr, ##__VA_ARGS__)

//create by keefor on 20190717
//C++ Call C#
class Debug {
public:
    static void(*Log)(char* message, int iSize);
    static void L(char* msg, ...);
};// C# call C++
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API InitCSharpDelegate(void(*Log)(char* message, int iSize));