#pragma once
#include "SingleTon.h"
#include <mutex>
#include <string>
#include <fcntl.h>
using std::lock_guard;
using std::mutex;
using std::string;

/* 日志等级 */
enum LogLevel
{
    LOG_LEVEL_NONE,    /* none level */
    LOG_LEVEL_ERROR,   /* error level */
    LOG_LEVEL_WARNING, /* warning level */
    LOG_LEVEL_DEBUG,   /* debug level */
    LOG_LEVEL_INFO,    /* info level */
};

/* 日志输出地 */
enum LogTarget
{
    LOG_TARGET_NONE = 0x00,
    LOG_TARGET_CONSOLE = 0x01,
    LOG_TARGET_FILE = 0x10,
};

// class Logger
// {
// public:
//     void init(LogLevel, LogTarget);
//     void destory();
//     int createFile();

// public:
//     static void writeLog(
//         LogLevel level,
//         unsigned char *fileName,
//         unsigned char *functionName,
//         unsigned int lineNum,
//         char *format,
//         ...);
//     static void outputToTarget();

// public:
//     LogLevel getLogLevel();
//     LogTarget getLogTarget();
//     void setLogLevel(LogLevel level);
//     void setLogTarget(LogTarget target);

// public:
//     static Logger *getInstance();

// private:
//     Logger() { init(LOG_LEVEL_NONE, LOG_TARGET_FILE); };
//     ~Logger(){};

// private:
//     static Logger *g_instance;
//     static mutex m_mutex;
//     static string logBuffer;
//     static FILE *m_file;
//     LogLevel m_level;
//     LogTarget m_target;
// };

/* 单例log类 */
class Log : public SingleTon<Log>
{
    friend class SingleTon<Log>;

public:
    Log(const Log &) = delete;
    Log &operator=(const Log &) = delete;
    ~Log()
    {
        // cout << "deconstruct Log" << endl;
    }

private:
    Log(){
        // cout << "construct Log" << endl;
    };

private:
    /* own data member */
    static mutex m_mutex;
    static string logBuffer;
    static FILE *m_file;
    LogLevel m_level;
    LogTarget m_target;

public:
    /* get and set value */
    LogLevel getLogLevel();
    LogTarget getLogTarget();
    void setLogLevel(LogLevel level);
    void setLogTarget(LogTarget target);

public:
    /* init data */
    void init(LogLevel level, LogTarget target);
    void destory();
    int createFile();

public:
    /* output */
    static void writeLog(
        LogLevel level,
        unsigned char *fileName,
        unsigned char *functionName,
        unsigned int lineNum,
        char *format,
        ...);
    static void outputToTarget();
    void dosomething();
};