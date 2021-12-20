#include "../include/Log.h"
namespace std
{
    /* 静态成员初始化 */
    string Log::logBuffer = "";
    FILE *Log::m_file = nullptr;
    RWLock Log::g_RWlock;
    char Log::g_logStoreFileName[24] = {};
    unsigned int Log::currentLineNum = 0;
    unsigned int Log::maxSuffix = 0;
    static const char *LOGLEVELARRAY[] = {
        "NONE",
        "ERROR",
        "WARNING",
        "DEBUG",
        "INFO",
    };
    /******************************************************************************
     * function Log::init
     * @ breif: init for logger, if its a file target, get and set data for file
     * @ arguments: log level, target
     * @ return: none
     ******************************************************************************/
    void Log::init(LogLevel level, LogTarget target)
    {
        m_level = level;
        m_target = target;
        if (target & LOG_TARGET_FILE)
        {
            /* 对静态成员操作，加锁 */
            lock_guard_WR lock(g_RWlock);
            /* 如果是输出到文件，设置文件名 */
            getMaxSuffix();
            setLogStoreFileName(maxSuffix);
            createFile();
        }
    }
    /******************************************************************************
     * function Log::closeFilePtr
     * @ breif: close the log file ptr
     * @ arguments: none
     * @ return: none
     ******************************************************************************/
    void Log::closeFilePtr()
    {
        if (m_file != nullptr)
        {
            fclose(m_file);
            m_file = nullptr;
#ifdef DEBUG
            fprintf(stdout, "close file log...\n");
#endif
        }
    }
    /******************************************************************************
     * function Log::createFile
     * @ breif: open and get the log file ptr
     * @ arguments: none
     * @ return: none
     ******************************************************************************/
    int Log::createFile()
    {
        /* create directory log */
        if (access(logDirectory, 0) != 0)
        {
            if (mkdir(logDirectory, 0755) == -1)
            {
#ifdef DEBUG
                fprintf(stdout, "mkdir error!\n");
#endif
                return FAILED;
            }
        }
        char logFilePatg[256];
        sprintf(logFilePatg, "%s/%s", logDirectory, g_logStoreFileName);
        m_file = fopen(logFilePatg, "ar+");
        if (m_file == nullptr)
        {
#ifdef DEBUG
            fprintf(stdout, "touch file log error\n");
#endif
            return FAILED;
        }
        return SUCCESS;
    }

    /******************************************************************************
     * function Log::writeLog
     * @ breif: do some format work and write the log to file
     * @ arguments: const char *format is to format the arguments(...)
     * @ return: none
     ******************************************************************************/
    void Log::writeLog(
        LogLevel level,
        const char *fileName,
        const char *functionName,
        unsigned int lineNum,
        const char *format,
        ...)
    {
        lock_guard_WR lock(g_RWlock);
        /* log time */
        time_t tt;
        time(&tt);
#ifdef GMTIME
        struct tm *currentTime = gmtime(&tt);
        char *timeStr = asctime(currentTime);
        // printf("gmtime: %s", asctime(timeStr));
#else
#ifdef DEBUG
        printf("localtime: %s", ctime(&tt));
#endif
        char *timeBuf = ctime(&tt);
        timeBuf[strlen(timeBuf) - 1] = '\0';
#endif
        /* log level */
#ifdef DEBUG
        cout << LOGLEVELARRAY[level] << endl;
#endif
        const char *logLevelBuf = LOGLEVELARRAY[level];
        /* log info */
        char localInfoBuf[256];
        sprintf(localInfoBuf, "[%s][PID:%4d][TID:%4lu][%s][%-s][%s:%4d]",
                timeBuf,
                getpid(),
                pthread_self(),
                logLevelBuf,
                fileName,
                functionName,
                lineNum);
#ifdef DEBUG
        cout << localInfoBuf << endl;
#endif
        /* log body */
        char content[256];
        va_list ap;
        va_start(ap, format);
        vsnprintf(content, 256, format, ap);
        va_end(ap);
        /* merge all the buffer */
        logBuffer = string(localInfoBuf) + string(content) + string("\n");
#ifdef DEBUG
        cout << logBuffer;
#endif
        outputToTarget();
    }
    /******************************************************************************
     * function Log::outputToTarget
     * @ breif: choose a way to output, you can choose both
     * @ arguments: none
     * @ return: none
     ******************************************************************************/
    void Log::outputToTarget()
    {
        if (Log::getInstance()->getLogTarget() & LOG_TARGET_FILE)
        {
            /* log to file */
            fputs(logBuffer.c_str(), m_file);
            /* log file line num increase */
            if (++currentLineNum >= 5000)
            {
                /* flush to disk */
                // syncfs(fileno(m_file));
                fflush(m_file);
                closeFilePtr();
                setLogStoreFileName(++maxSuffix);
                setMaxSuffix();
                createFile();
                currentLineNum = 0;
            }
        }
        if (Log::getInstance()->getLogTarget() & LOG_TARGET_CONSOLE)
        {
            /* log to console */
            fprintf(stdout, "%s", logBuffer.c_str());
        }

        logBuffer.clear();
    }
    /******************************************************************************
     * function Log::setLogStoreFileName
     * @ breif: set the log file name
     * @ arguments: suffix
     * @ return: none
     ******************************************************************************/
    void Log::setLogStoreFileName(const unsigned int suffix)
    {
        /* set log store file name */
        strcpy(g_logStoreFileName, LOGFILENAME);
        if (suffix == 0)
        {
            sprintf(g_logStoreFileName, "%s.log%c", g_logStoreFileName, '\0');
        }
        else
        {
            sprintf(g_logStoreFileName, "%s_%d.log%c", g_logStoreFileName, suffix, '\0');
        }
    }
    /******************************************************************************
     * function Log::getMaxSuffix
     * @ breif: get the log file suffix from log/maxSuffix
     * @ arguments: none
     * @ return: none
     ******************************************************************************/
    void Log::getMaxSuffix()
    {
        char maxSuffixPath[20];
        sprintf(maxSuffixPath, "%s/maxSuffix", logDirectory);
        int fd = open(maxSuffixPath, O_RDONLY | O_CREAT, 0755);
        char buffer[4];
        int noneVal = read(fd, buffer, 4);
        if (strlen(buffer) != 0)
        {
            /* not empty maxSuffix file */
            maxSuffix = atoi(buffer);
        }
        else
        {
            maxSuffix = 0;
        }
        close(fd);
    }
    /******************************************************************************
     * function Log::setMaxSuffix
     * @ breif: update the maxSuffix to file log/maxSuffix
     * @ arguments: none
     * @ return: none
     ******************************************************************************/
    void Log::setMaxSuffix()
    {
        char maxSuffixPath[20];
        sprintf(maxSuffixPath, "%s/maxSuffix", logDirectory);
        int fd = open(maxSuffixPath, O_WRONLY | O_CREAT, 0755);
        char buffer[4];
        sprintf(buffer, "%d", maxSuffix);
        /* 清空文件 */
        int noneVal = ftruncate(fd, 0);
        /* 重新设置文件偏移量 */
        lseek(fd, 0, SEEK_SET);
        noneVal = write(fd, buffer, 4);
        close(fd);
    }
    /******************************************************************************
     * function Log::getLogLevel
     * @ breif: get the log level
     * @ arguments: none
     * @ return: none
     ******************************************************************************/
    LogLevel Log::getLogLevel()
    {
        return m_level;
    }
    /******************************************************************************
     * function Log::getLogTarget
     * @ breif: get the log target
     * @ arguments: none
     * @ return: none
     ******************************************************************************/
    LogTarget Log::getLogTarget()
    {
        return m_target;
    }
    /******************************************************************************
     * function Log::setLogLevel
     * @ breif: set the log level
     * @ arguments: level
     * @ return: none
     ******************************************************************************/
    void Log::setLogLevel(LogLevel level)
    {
        m_level = level;
    }
    /******************************************************************************
     * function Log::setLogTarget
     * @ breif: set the log target
     * @ arguments: target
     * @ return: none
     ******************************************************************************/
    void Log::setLogTarget(LogTarget target)
    {
        m_target = target;
    }
    /******************************************************************************
     * function Log::dosomething
     * @ breif: test for the logger
     * @ arguments: none
     * @ return: none
     ******************************************************************************/
    void Log::dosomething()
    {
        cout << "output" << endl;
    }
}