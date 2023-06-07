#include "ThreadManager.h"
#include "../Logger/Logger.h"
#include <mutex>
namespace threading
{
#define default_thread_pool_size 4
	static ThreadManager* instance = nullptr;
	uint32_t ThreadManager::defaultPoolSize = default_thread_pool_size;
	ThreadManager::ThreadManager() : m_pPool(nullptr)
	{
	}

	ThreadManager::ThreadManager(uint32_t size) : m_pPool(nullptr)
	{
		m_pPool = new ThreadPool(size);
	}
	ThreadManager::~ThreadManager()
	{
		safe_delete(m_pPool);
		Log("Thread Manager Destroyed");
	}

	void ThreadManager::AddTask(const ThreadTask & task)
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		m_pPool->enqueue(task);
	}

	ThreadManager* ThreadManager::GetInstance()
	{
		if (!instance)
		{
			instance = new ThreadManager(defaultPoolSize);
		}
		return instance;
	}
	void ThreadManager::Destroy()
	{
		safe_delete(instance);
	}

	void ThreadManager::SetThreadCount(uint32_t count)
	{
		defaultPoolSize = count;
		/*if (instance)
		{
			LogError("ThreadManager::SetThreadCount should be called before GetInstance or sThreadManager is called!");
		}*/
	}
}
