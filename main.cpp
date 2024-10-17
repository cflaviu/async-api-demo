#include "async_api.hpp"
#include <iostream>

int main()
{
    async_api api(4u);

    auto future_square_result = api.square(5);
    auto future_concat_result = api.concat("Hello, ", "World!");

    std::cout << "5 squared: " << future_square_result.get() << std::endl;
    std::cout << "concatenated string: " << future_concat_result.get() << std::endl;

    api.stop();

    return 0;
}
