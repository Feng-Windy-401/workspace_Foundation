#include "./logger.h"
#include <thread>
#include <vector>

/**
 * @brief 任务函数，用于在多个线程中记录日志
 * @param logger 日志记录器对象
 * @param threadId 线程ID
 */
void task(Logger &logger, int threadId)
{
    for(int i = 0; i < 5; ++i)
    {
        logger.log(INFO, "Thread " + std::to_string(threadId), "Log message " + std::to_string(i));
        logger.log(WARNING, "Thread " + std::to_string(threadId), "Log message " + std::to_string(i));
        logger.log(ERROR, "Thread " + std::to_string(threadId), "Log message " + std::to_string(i));
    }
}

int main()
{
    Logger logger("log.txt");

    std::vector<std::thread> threads;
    for(int i = 0; i < 5; ++i)
    {
        threads.emplace_back(task, std::ref(logger), i);
    }

    for(auto &t : threads)
    {
        t.join();
    }
    
    return 0;
}