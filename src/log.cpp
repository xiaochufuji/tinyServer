#include "../include/log.h"
/* 静态成员初始化 */
mutex Log::m_mutex;
string Log::logBuffer = "";
FILE *Log::m_file = nullptr;

void Log::init(LogLevel level, LogTarget target)
{
    m_level = level;
    m_target = target;
    createFile();
}
void Log::destory()
{
    if (m_file != nullptr)
    {
        fclose(m_file);
    }
}
int Log::createFile()
{
}
void Log::writeLog(
    LogLevel level,
    unsigned char *fileName,
    unsigned char *functionName,
    unsigned int lineNum,
    char *format,
    ...)
{
}
void Log::outputToTarget()
{
    
}
LogLevel Log::getLogLevel()
{
    return m_level;
}
LogTarget Log::getLogTarget()
{
    return m_target;
}
void Log::setLogLevel(LogLevel level)
{
    m_level = level;
}
void Log::setLogTarget(LogTarget target)
{
    m_target = target;
}

void Log::dosomething()
{
    cout << "output" << endl;
}
