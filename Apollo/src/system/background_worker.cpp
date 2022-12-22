#include "background_worker.h"

background_worker::background_worker()
	: m_thread{ &background_worker::work, this }
{}

background_worker::~background_worker()
{
	{
		std::scoped_lock<std::mutex> lock{ m_queue_mutex };
		m_exit = true;
		m_queue_pending.notify_one();
	}

	m_thread.join();
}

void background_worker::add_job(const std::function<void()>& value)
{
	std::scoped_lock<std::mutex> lock{ m_queue_mutex };
	m_job_queue.push(value);
	m_queue_pending.notify_one();
}

size_t background_worker::get_num_pending_jobs() const
{
	std::scoped_lock<std::mutex> lock{ m_queue_mutex };

	return m_job_queue.size();
}

void background_worker::work()
{
	std::function<void()> job;
	
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock{ m_queue_mutex };
			m_queue_pending.wait(lock, [this]() -> bool { return m_exit || !m_job_queue.empty(); });

			if (m_exit) return;

			job = m_job_queue.front();
			m_job_queue.pop();
		}

		job();
	}
}
