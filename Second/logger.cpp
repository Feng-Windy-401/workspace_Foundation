#include "./logger.h"

/**
 * @brief Logger类的构造函数，初始化日志文件并启动日志线程
 * @param filename 日志文件名
 * @throws std::runtime_error 如果无法打开日志文件
 */
Logger::Logger(const std::string &filename)
{
    std::cout << "Initializing Logger with file: " << filename << std::endl;
    m_logFile.open(filename, std::ios::out | std::ios::app);
    if (!m_logFile.is_open())
    {
        throw std::runtime_error("Unable to open log file");
    }
    isRunning = true;
    m_thread = std::thread(&Logger::WriteToFile, this);
}

/**
 * @brief Logger类的析构函数，停止日志线程并关闭日志文件
 * 确保所有资源得到正确清理
 */
Logger::~Logger()
{
    // 加锁，无需管啥时候释放
    std::unique_lock<std::mutex> lock(m_mutex);
    isRunning = false;
    m_cond.notify_all();

    // 等价上述写法
    // {
    //     std::lock_guard<std::mutex> lock(m_mutex);
    //     isRunning = false;
    //     m_cond.notify_all();
    // }
    
    if (m_thread.joinable())
    {
        m_thread.join();
    }

    if (m_logFile.is_open())
    {
        m_logFile.close();
    }

    std::cout << "Logger destroyed and log file closed." << std::endl;
}

/**
 * @brief 获取日志级别对应的字符串表示
 * @param level 日志级别
 * @return 对应的字符串表示
 */
std::string Logger::getLevel(LogLevel level)
{
    switch (level)
    {
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

/*
 * @brief 记录日志
 * @param level 日志级别
 * @param threadName 线程名
 * @param message 日志消息
 */
void Logger::log(LogLevel level, const std::string &threadName, const std::string &message)
{
    std::string logEntry = "[" + getLevel(level) + "] [" + threadName + "]: " + message;
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_logQueue.push(logEntry);
        lock.unlock();
    }
    m_cond.notify_one();
}

/*
 * @brief 写日志到文件
 */
void Logger::WriteToFile(void)
{
    std::cout << "Logger thread started, waiting for log entries..." << std::endl;
    while (isRunning)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock, [this]()
                    { return !m_logQueue.empty() || !isRunning; });

        while (!m_logQueue.empty())
        {
            std::string logEntry = m_logQueue.front();
            m_logQueue.pop();

            if (m_logFile.is_open())
            {
                std::cout << logEntry << std::endl; // Optional: Print to console
                m_logFile << logEntry << std::endl;
                m_logFile.flush(); // Ensure the log is written to file immediately
            }
        }
    }
}
