#include "UnityDebug.h"


//create by keefor on 20190717
void(*Debug::Log)(char* message, int iSize);
void Debug::L(char* fmt, ...) {

    if (Debug::Log == nullptr)return;

    char acLogStr[1024];// = { 0 };

    va_list ap;

    va_start(ap, fmt);
    vsprintf(acLogStr, fmt, ap);
    va_end(ap);

    Debug::Log(acLogStr, strlen(acLogStr));
}


extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API InitCSharpDelegate(void(*Log)(char* message, int iSize)) {
    Debug::Log = Log;
    //UnityLog("Cpp Message:Log has initialized");
}
