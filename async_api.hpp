#include "thread_pool.hpp"

class async_api
{
public:
    async_api(std::size_t thread_count);

    async_api() = delete;
    ~async_api() = default;

    async_api(async_api&&) = default;
    async_api(const async_api&) = delete;

    async_api& operator=(async_api&&) = default;
    async_api& operator=(const async_api&) = delete;

    void stop();

    std::future<int> square(int x);

    std::future<std::string> concat(const std::string& a, const std::string& b);

private:
    thread_pool thread_pool_;
};
