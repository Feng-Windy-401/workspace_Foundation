# C++11多线程汇总



## 一.基础

### 1.核心头文件

```c
#include <thread>	//线程核心: std::thread
#include <mutex>	//互斥锁核心：std::mutex
#include <iostream>
```



### 2.线程的基本概念

- 线程是程序执行的最小单位，一个进程可以拥有多个线程，共享进程的内存空间
- C++11 直接用标准库创建线程，替代传统 pthread 库，跨平台、更简洁



### 3.线程创建与管理的核心操作

- 创建: `std::thread 线程名(函数名， 函数参数)`
- 等待: `线程名.join();`——主线程等待子线程执行完毕再继续
- 分离:`线程名.detach();`——子线程与主线程分离，独立运行，变成后台守护进程，极易访问非法内存



### 4.竞态条件与互斥锁

- 竞态条件: 多个线程同时访问 / 修改共享资源（如全局变量、控制台打印），导致结果混乱
- 互斥锁`std::mutex`：通过**加锁**/**解锁**保证同一时间只有一个线程访问共享资源，解决竞态条件
  - 加锁: `mutex对象.lock();`
  - 解锁:`mutex对象.unlock();`



------



## 二.进阶

### 1.使用lambda创建线程

```cpp
int main(){
    std::thread t([](){
       std::cout << "Lambda线程运行ing" << std::endl; 
    });
    
    t.join();
    return 0;
}
```



### 2.线程函数的参数传递

`std:;thread`默认是拷贝参数，即使函数声明为引用

必须用`std:::ref()`包装才能传递真正的引用

#### (1)基础传值

```cpp
void print(int num, const std::string& str) {
    std::cout << num << " " << str << "\n";
}

int main() {
    int a = 10;
    std::string s = "hello";
    // 直接传参，自动拷贝
    std::thread t(print, a, s); 
    t.join();
    return 0;
}
```

#### (2)传引用

```cpp
void add(int &num) {num += 10;}

int main()
{
	int a = 10;
    std:thread t1(add, std::ref(a));
    t.join();
    std::cout << a;
    return 0;
}
```

#### (3)传递类成员函数

需要传入对象指针/引用 + 函数参数：

```cpp
class Test{
  public:
    	void show(int x){
            std::cout << x << std::endl;
        }
};

int main()
{
    std::unique_ptr obj = std::make_unique<Test>();
    std::thread t(&Test::show, &*obj, 100);
    t.join();
    return 0;
}
```



### 3.RAll锁`std::lock_guard`

资源获取即初始化（RAll）：创建时自动加锁，析构时自动解锁，绝对不会死锁/忘解锁

```cpp
void add_count(){
    for(int i = 0; i < 10000; ++i)
    {
        std::lock_guard<std::mutex> lock(mtx);
        count++;
    }
}
```



### 4.`std::condition_variable`：条件变量（线程同步）

互斥锁解决数据竞争，条件变量解决线程等待/通知

核心API：

- `wait(lock)`释放锁，线程阻塞等待
- `notify_one()`唤醒一个等待的线程
- `notify_all()`唤醒所有等待的线程

> 注意：条件变量必须搭配`std::unique_lock`使用

**示例：生产者-消费者模型**

```cpp
#include <queue>
#include <condition_variable>

std::queue<int> q;          // 共享队列
std::mutex mtx;             // 互斥锁
std::condition_variable cv;  // 条件变量

// 生产者：往队列放数据
void producer() {
    for (int i = 1; i <= 5; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        q.push(i);
        std::cout << "生产：" << i << "\n";
        cv.notify_one(); // 唤醒消费者
    }
}

// 消费者：从队列取数据
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        // 等待队列非空，否则阻塞
        cv.wait(lock, [](){ return !q.empty(); });
        
        int val = q.front(); q.pop();
        std::cout << "消费：" << val << "\n";
        lock.unlock();

        if (val == 5) break;
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join(); t2.join();
    return 0;
}
```



### 5.`std::atomic`原子操作

对于简单的共享变量，用`std::atomic`可以不用锁实现线程安全，效率比mutex高很多

**示例：原子计数（无锁，绝对安全）**

```cpp
#include <atomic>
// 原子整数，自带线程安全
std::atomic<int> count = 0; 

void add_count() {
    for (int i = 0; i < 100000; ++i) {
        count++; // 原子操作，无需锁！
    }
}

int main() {
    std::thread t1(add_count);
    std::thread t2(add_count);
    t1.join(); t2.join();
    std::cout << count; // 输出 200000，完美
    return 0;
}
```



### 6.`thread_local`线程局部存储

`thread_local`修饰的变量：每个线程有独立的副本，互不干扰，无需加锁

```cpp
// 全局变量，但每个线程独立一份
thread_local int num = 0; 

void func() {
    num++;
    std::cout << "线程num：" << num << "\n";
}

int main() {
    std::thread t1(func); // 输出1
    std::thread t2(func); // 输出1
    t1.join(); t2.join();
    return 0;
}
```

------



## 三.智能锁

### 1.使用智能锁的目的

手动使用`mutex.lock() \ mutex.unlock()`存在一些致命问题：

- 程序异常\提前退出，忘记`unlock`造成死锁
- 多分支逻辑，造成加解锁不匹配，进而程序卡死

采用智能锁，构造加锁、析构解锁，自动兜底



### 2.智能锁的分类

- `std::lock_guard`，极简RAll，构造加锁、析构解锁，不可手动解锁
- `std::unique_lock`，灵活RAll，支持手动lock/unlock、延迟锁



### 3.智能锁的核心用法

```cpp
#include<mutex>
std::mutex mtx;

/* lock_guard(常规使用) */
std::lock_guard<std::mutex> lock(mtx);

/* unique_lock(灵活使用) */
std::unique_lock<std::mutex> lock(mtx);
```



### 4.`std::lock_guard`实战

```cpp
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int g_counter = 0;
std::mutex mtx;

void task(int id)
{

    std::lock_guard<std::mutex> lock(mtx); // 这里加锁是对整个线程的执行都加锁，保证了打印也不会出项竞态条件
    std::cout << "Task " << id << " is running." << std::endl;

    std::cout << "Task [" << id << "]{" << g_counter << "}" << std::endl;

    for (int i = 0; i < 6; ++i)
    {
        ++g_counter;
        std::cout << "Task [" << id << "]{" << g_counter << "}" << std::endl;
    }

    std::cout << "Task " << id << " is finished." << std::endl;
}

int main()
{
    std::thread t1(task, 1);
    t1.join();

    std::vector<std::thread> threads;

    for (int i = 2; i < 4; ++i)
    {
        threads.emplace_back(task, i);
    }

    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}
```

------



## 四.实战

生产者——消费者模型，多个生产者，单个消费者。多个线程于日志队列中新增数据，单个消费者于日志队列中取出数据，写入日志文件。

==头文件==——`logger.h`

```c++
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
```



==源文件==——`logger.cpp`

```c++
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

```

 ==主文件==——`main.cpp`

```c++
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
```

==编译命令==：

```shell
g++ mian.cpp logger.cpp -o logger -pthread -std=c++11
```

