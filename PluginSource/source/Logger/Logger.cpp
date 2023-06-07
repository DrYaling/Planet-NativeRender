#include "Logger.h"
#include <mutex>
#include <map>
#include <string>
using std::make_pair;
namespace logger
{
#define LOG_TRACE
#define log_buffer_size 2048
	static char Logger_cbuffer[log_buffer_size] = { 0 };
	static std::string logger_buff;
	std::mutex _logMtx;
	static CPPLogCallback logCallBack = nullptr;
	static uint32_t sLogLevel = 4;
	static std::map<int32_t, std::string> loggerMap;
#ifdef WIN32
#define strsplit strtok_s
#else
#define strsplit strtok_r
#endif
#if false
	static void printStackTrace()
	{
		int size = 16;
		void * array[16];
		int stack_num = backtrace(array, size);
		char ** stacktrace = backtrace_symbols(array, stack_num);
		for (int i = 0; i < stack_num; ++i)
		{
			printf("%s\n", stacktrace[i]);
		}
		free(stacktrace);
	}
#elif false
	static void printStackTrace()
	{
		unsigned int   i;
		void         * stack[100];
		unsigned short frames;
		SYMBOL_INFO  * symbol;
		HANDLE         process;

		process = GetCurrentProcess();

		SymInitializeW(process, NULL, TRUE);

		frames = CaptureStackBackTrace(0, 100, stack, NULL);
		symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		char buffer[256] = { 0 };
		for (i = 0; i < frames; i++)
		{
			SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
			memset(buffer, 256, 0);
			sprintf_s(buffer, "%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
			//printf("%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
			buff.append(buffer);
			buff.append("\n");
		}

		free(symbol);
	}

#else
	void printStackTrace()
	{

	}
#endif
	static size_t exclude_dir_len = strlen(solutionDir()) - sizeof("define.h");
	static bool loggerInited = false;
	void ClearLogger()
	{
		logCallBack = nullptr;
		if (loggerInited)
			return;
		loggerMap.clear();
		loggerMap.insert(make_pair<int32_t, std::string>(3, "[TRACE]"));
		loggerMap.insert(make_pair<int32_t, std::string>(3, "[DEBUG]"));
		loggerMap.insert(make_pair<int32_t, std::string>(2, "[WARN]"));
		loggerMap.insert(make_pair<int32_t, std::string>(1, "[ERROR]"));
		loggerInited = true;
	}
	void SetLogCallBack(CPPLogCallback cb)
	{
		logCallBack = cb;
	}
#if WIN32
	DWORD start, stop;
	static std::string profilerContent;
#endif
	void ProfilerStart(const char * content)
	{
#if WIN32
		start = GetTickCount();
		profilerContent = content;
#endif
	}
	void ProfilerEnd(uint32_t minDiff)
	{
#if WIN32
		stop = GetTickCount();
		if (profilerContent.size() <=0)
		{
			profilerContent = "";
		}
		if (stop - start >= minDiff)
		{
			LogFormat("%s cost time %d ms", profilerContent.c_str(), stop - start);
		}
		profilerContent.resize(0);
#endif
	}
	int LogContent(LoggerType eType, const char	* fp, int line, const char* func, const char* format, ...)
	{
		//如果日志级别不够就不打印,降低互斥锁的性能损耗
		if (eType > sLogLevel)
			return 0;
		std::lock_guard<std::mutex> lck(_logMtx);
		logger_buff.clear();
		memset(Logger_cbuffer, 0, sizeof(Logger_cbuffer));
		va_list valist;
		va_start(valist, format);
		vsprintf(Logger_cbuffer, format, valist);
		logger_buff.append("[Native]");
		const auto logType = loggerMap.find(eType);
		if(logType != loggerMap.end())
			logger_buff.append(logType->second);
		logger_buff.append(": ");
		logger_buff.append(Logger_cbuffer);
		logger_buff.append("\n");
		va_end(valist);
#ifdef LOG_TRACE
		if (exclude_dir_len < 0 || exclude_dir_len > 255)
		{
			exclude_dir_len = 0;
		}
		memset(Logger_cbuffer, 0, sizeof(Logger_cbuffer));
		sprintf(Logger_cbuffer, "in file %s,function %s at line %d\n", fp + exclude_dir_len, func, line);
		logger_buff.append(Logger_cbuffer);
#endif // LOG_TRACE
		//int logtype = -1;
		const auto msg = logger_buff.c_str();
		uint32_t size = strlen(msg);
		switch (eType)
		{
			case LoggerType::LOGGER_ERROR:
				if (nullptr != logCallBack)
				{
					logCallBack(msg, eType);
				}
				else
					printf("%s", msg);
				break;
			case LoggerType::LOGGER_WARN:
				if (sLogLevel >= eType && nullptr != logCallBack)
				{
					logCallBack(msg, eType);
				}
				else
					printf("%s", msg);
				break;
			default:
				if (sLogLevel >= eType && nullptr != logCallBack)
				{
					logCallBack(msg, eType);
				}
				else
					printf("%s", msg);
				break;
		}
		return 0;
	}

	void SetLogLevel(uint32_t logLevel)
	{
		TraceFormat("log level %d", logLevel);
		sLogLevel = logLevel;
	}
}
