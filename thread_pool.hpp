#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class thread_pool
{
public:
    thread_pool(std::size_t thread_count);

    thread_pool() = delete;
    thread_pool(thread_pool&&) = default;
    thread_pool(const thread_pool&) = delete;
    ~thread_pool() = default;

    thread_pool& operator=(thread_pool&&) = default;
    thread_pool& operator=(const thread_pool&) = delete;

    bool is_running() const noexcept { return !stop_; }
    void stop();

    // Submit a task to the pool and get a future result
    template <class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        using return_type = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        auto result = task->get_future();

        {
            // Lock to safely push the task to the queue
            std::unique_lock<std::mutex> lock(task_queue_mutex_);
            if (stop_)
            {
                throw std::runtime_error("thread_pool is stopping, cannot submit new task_queue_.");
            }

            task_queue_.emplace([task]() { (*task)(); });
        }

        condition_.notify_one(); // Notify one worker to wake up and process the task
        return result;
    }

private:
    void loop() noexcept;
    bool execute();

    std::vector<std::thread> workers_;             // Workers pool
    std::queue<std::function<void()>> task_queue_; // Task queue
    std::mutex task_queue_mutex_;                  // Mutex for synchronizing task access
    std::condition_variable condition_;            // Condition variable for task management
    std::atomic_bool stop_ {};                     // Flag for stopping the thread pool
};
