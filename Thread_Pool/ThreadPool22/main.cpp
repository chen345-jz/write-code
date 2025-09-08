#include "ThreadPool.h"
#include <iostream>

int main()
{
    ThreadPool pool(4);
    for (size_t i = 0; i < 10; i++)
    {
        auto rsfuture = pool.enques([](int a, int b) -> int
                                    {
            std::cout << "当前线程" << std::this_thread::get_id() << std::endl;
            return a + b; }, 10 * i, 10 * i);
        std::cout << "thread rs: " << rsfuture.get() << std::endl;
    }
    return 0;
}