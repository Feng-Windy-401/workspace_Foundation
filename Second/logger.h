#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <condition_variable>
#include <fstream>
#include <queue>

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    std::mutex m_mutex;                     // 互斥锁对象
    std::condition_variable m_cond;         // 条件变量对象
    std::thread m_thread;                   // 写日志线程对象
    bool isRunning;                         // 标志位，指示日志线程是否正在运行
    std::fstream m_logFile;                 // 日志文件对象
    std::queue<std::string> m_logQueue;     // 日志队列

    std::string getLevel(LogLevel level);
    void WriteToFile(void);                 // 写日志线程函数

public:
    Logger(const std::string &filename);    // 构造函数，接受日志文件名作为参数
    ~Logger();                              // 析构函数，负责清理资源
    void log(LogLevel level, const std::string &threadName, const std::string &message);
};

#endif