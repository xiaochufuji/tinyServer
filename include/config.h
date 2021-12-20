#pragma once
/* choose Mode(debug or release) */
#define MODE 0
#if MODE
#define DEBUG
#else
#define RELEASE
#endif

/* something global definition */
#define SUCCESS (1)
#define FAILED (0)

/* Macro for LOG */
#define LOG(level, fileName, functionName, lineNum, format, ...) \
    Log::getInstance()->writeLog(level, fileName, functionName, lineNum, format, ##__VA_ARGS__)
#define LOGSETLEVEL(level) \
    Log::getInstance()->setLogLevel(level);
#define LOGSETTARGET(target) \
    Log::getInstance()->setLogTarget(target);
#define LOGGETLEVEL() \
    Log::getInstance()->getLogLevel();
#define LOGGETTARGET() \
    Log::getInstance()->getLogTarget();

/* relate to bulid directory */
const char *logDirectory = "../log";

/* log file name */
#define LOGFILENAME ("test")