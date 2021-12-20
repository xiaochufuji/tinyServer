#pragma once
#include "config.h"
#include "SingleTon.h"
#include "RWLock.h"
#include <mutex>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

namespace std
{
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
        LOG_TARGET_BOTH = 0x11,
    };

    /* 单例log类 */
    class Log : public SingleTon<Log>
    {
        friend class SingleTon<Log>;

    public:
        Log(const Log &) = delete;
        Log &operator=(const Log &) = delete;
        ~Log()
        {
#ifdef DEBUG
            cout << "deconstruct Log" << endl;
#endif
            closeFilePtr();
        }

    private:
        Log()
        {
#ifdef DEBUG
            cout << "construct Log" << endl;
#endif
            init(LOG_LEVEL_NONE, LOG_TARGET_FILE);
        };

    private:
        /* own data member */
        static string logBuffer;
        static FILE *m_file;
        static RWLock g_RWlock;
        static char g_logStoreFileName[24];
        static unsigned int currentLineNum;
        static unsigned int maxSuffix;
        LogLevel m_level;
        LogTarget m_target;

    private:
        /* init data */
        void init(LogLevel level, LogTarget target);
        static void closeFilePtr();
        static int createFile();

    public:
        /* output */
        static void writeLog(
            LogLevel level,
            const char *fileName,
            const char *functionName,
            unsigned int lineNum,
            const char *format,
            ...);
        static void outputToTarget();

    private:
        static void setLogStoreFileName(const unsigned int suffix);
        static void getMaxSuffix();
        static void setMaxSuffix();

    public:
        /* get and set value */
        LogLevel getLogLevel();
        LogTarget getLogTarget();
        void setLogLevel(LogLevel level);
        void setLogTarget(LogTarget target);
        void dosomething();
    };
}