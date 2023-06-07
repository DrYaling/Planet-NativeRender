#ifndef _logger_h
#define _logger_h
#include <iostream>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../define.h"
#include "../Unity/IUnityInterface.h"
#define LOG_IN_THREAD 0
//#define LOG_TYPE_AS_TITLE
#define LOG_SWITCH 1
#ifdef UNITY_WIN
#include <Windows.h>
#include "DbgHelp.h"
#else
//#include <execinfo.h>
#endif


enum LoggerType
{
	LOGGER_ERROR = 1,
	LOGGER_WARN = 2,
	LOGGER_LOG = 3,
	LOGGER_TRACE = 4,
};
#if LOG_SWITCH
#define TraceFormat(format, ...) logger::LogContent(LoggerType::LOGGER_TRACE,__FILE__, __LINE__,__FUNCTION__, format, __VA_ARGS__)
#define Trace(format) logger::LogContent(LoggerType::LOGGER_TRACE,__FILE__, __LINE__,__FUNCTION__,  format)
#define LogFormat(format, ...) logger::LogContent(LoggerType::LOGGER_LOG,__FILE__, __LINE__,__FUNCTION__, format, __VA_ARGS__)
#define Log(format) logger::LogContent(LoggerType::LOGGER_LOG,__FILE__, __LINE__,__FUNCTION__,  format)
#define LogWarningFormat(format, ...) logger::LogContent(LoggerType::LOGGER_WARN,__FILE__, __LINE__,__FUNCTION__,  format, __VA_ARGS__)
#define LogWarning(format) logger::LogContent(LoggerType::LOGGER_WARN,  format)
#define LogErrorFormat(format, ...) logger::LogContent(LoggerType::LOGGER_ERROR,__FILE__, __LINE__,__FUNCTION__,  format, __VA_ARGS__)
#define LogError(format) logger::LogContent(LoggerType::LOGGER_ERROR,__FILE__, __LINE__,__FUNCTION__,  format)
#else
#define TraceFormat(logType,format, ...) 
#define Trace(logType,format)
#define LogFormat(logType,format, ...) 
#define Log(logType,format)
#define LogWarningFormat(logType,format, ...) 
#define LogWarning(logType,format) 
#define LogErrorFormat(logType,format, ...)
#define LogError(logType,format) 
#endif

namespace logger
{
	extern "C"
	{
		//void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API logger(void(*Log)(const char* log, uint32_t size));
		typedef void(UNITY_INTERFACE_API*CPPLogCallback)(const char* log, int32_t log_level);
		int LogContent(LoggerType eType, const char	* f, int line, const char* func, const char* format, ...);
		void printStackTrace();
		void ClearLogger();
		void SetLogCallBack(CPPLogCallback cb);
		void ProfilerStart(const char* content);
		void ProfilerEnd(uint32_t minDiff = 0);
		void SetLogLevel(uint32_t logLevel);
	}
}

#endif
