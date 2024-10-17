#include "async_api.hpp"

async_api::async_api(std::size_t thread_count): thread_pool_(thread_count)
{
}

void async_api::stop()
{
    thread_pool_.stop();
}

std::future<int> async_api::square(int x)
{
    return thread_pool_.submit(
        [x]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
            return x * x;
        });
}

std::future<std::string> async_api::concat(const std::string& a, const std::string& b)
{
    return thread_pool_.submit(
        [a, b]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Simulate work
            return a + b;
        });
}
