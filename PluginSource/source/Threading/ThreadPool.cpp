#include "ThreadPool.h"
#include <sstream>

namespace threading
{
	struct ThreadExitLog
	{
		ThreadExitLog() = delete;
		explicit ThreadExitLog(const std::thread::id& threadId) :m_nthreadId(0)
		{
			std::ostringstream oss;
			oss << threadId;
			std::string stid = oss.str();
			m_nthreadId = std::stoull(stid);
		}
		~ThreadExitLog()
		{
			LogFormat("ThreadPool thread %lld exit", m_nthreadId);
		}
		uint64_t m_nthreadId;
	};
	ThreadPool::ThreadPool(size_t threads) :
		m_bStop(false)
	{
		LogFormat("ThreadPool start size %d", threads);
		for (size_t i = 0; i < threads; ++i)
		{
			m_vWorkers.emplace_back([this]
			{
				ThreadExitLog threadLog(std::this_thread::get_id());
				for (;;)
				{
					ThreadTask task;

					{
						std::unique_lock<std::mutex> lock(m_mutex);
						this->m_condition.wait(lock,[this]
						{
							return this->Predicate();
						});
						//Log("task thread awake");
						if (this->m_bStop && this->m_qTasks.empty())
							return;
						task = std::move(this->m_qTasks.front());
						this->m_qTasks.pop();
					}
					task();
					//Log("task stopped");
				}
			}
			);
		}
	}

	void ThreadPool::enqueue(const ThreadTask& task)
	{
		// don't allow enqueueing after stopping the pool
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			if (m_bStop)
			{
				LogError("enqueue on stopped ThreadPool");
				return;
			}
			m_qTasks.emplace(task);
		}
		m_condition.notify_one();
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_bStop = true;
		}
		m_condition.notify_all();
		for (std::thread &worker : m_vWorkers)
			worker.join();
	}

	bool ThreadPool::Predicate() const
	{
		//LogFormat("Predicate stopped %d, not empty %d",m_bStop, !m_qTasks.empty());
		return this->m_bStop || !this->m_qTasks.empty();
	}
}
