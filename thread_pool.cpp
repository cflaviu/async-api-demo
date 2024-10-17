#include "thread_pool.hpp"

thread_pool::thread_pool(std::size_t thread_count): stop_(false)
{
    for (; thread_count-- != 0u;)
    {
        workers_.emplace_back([this] { loop(); });
    }
}

bool thread_pool::execute()
{
    std::function<void()> task;

    {
        std::unique_lock<std::mutex> lock(task_queue_mutex_);
        condition_.wait(lock, [this] { return stop_ || !task_queue_.empty(); });
        if (stop_ && task_queue_.empty())
        {
            return false;
        }

        task = std::move(task_queue_.front());
        task_queue_.pop();
    }

    task();
    return true;
}

void thread_pool::loop() noexcept
{
    for (bool can_continue = true; can_continue;)
        try
        {
            can_continue = execute();
        }
        catch (const std::exception& ex)
        {
            // todo add logs
        }
        catch (...)
        {
            // todo add logs
        }
}

void thread_pool::stop()
{
    stop_ = true;
    condition_.notify_all();
    for (std::thread& worker: workers_)
    {
        worker.join(); // Join all threads
    }
}
