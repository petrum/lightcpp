#pragma once

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <sys/time.h>

std::string GetEnv(const char* pName, const char* pDefault);
enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG, logALL};
TLogLevel levelFromString(const std::string& level);
std::string nowTime();

inline const char* const logToString(TLogLevel level)
{
    static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "ALL"};
    return buffer[level];
}

class FileLog
{
 public:
    FileLog(TLogLevel level) : messageLevel(level)
    {
        os << std::setprecision(16);
    }
    ~FileLog()
    {
        std::ostringstream tmp;
        tmp << nowTime() << " [" << pthread_self() << "]";
        tmp << " " << logToString(messageLevel) << ": ";
        tmp << os.str();
        tmp << "\n";
        write(tmp.str());
    }
    std::ostringstream& getStream()
    {
        return os;
    }
    static FILE*& getFile()
    {
        static FILE* pStream = stderr;
        return pStream;
    }
    static TLogLevel& getReportingLevel()
    {
        static TLogLevel reportingLevel = levelFromString(GetEnv("LOG_LEVEL", "INFO"));
        return reportingLevel;
    }
    static void write(const std::string& msg)
    {   
        fprintf(getFile(), "%s", msg.c_str());
        fflush(getFile());
    }
private:
    FileLog(const FileLog&);
    FileLog& operator =(const FileLog&);
private:
    static TLogLevel reportingLevel;
    TLogLevel messageLevel;
    std::ostringstream os;
};

#define FILE_LOG(messageLevel) if (messageLevel > FileLog::getReportingLevel()) ; else FileLog(messageLevel).getStream()

template <typename T>
T enforce(T t, const char* e, const char* f, int n)
{
    if (!t)
    {
        std::ostringstream o;
        o << "The '" << e << "' expression has failed in " << f << ":" << n;
        throw std::runtime_error(o.str());
    }
}

#define ENFORCE(expr) enforce(expr, #expr, __FILE__, __LINE__)

#define STD_FUNC_BEGIN try {
#define STD_FUNC_END } catch (const std::exception& e) \
{\
    FILE_LOG(logERROR) << e.what();\
}\
catch (...)\
{\
    FILE_LOG(logERROR) << "Non standard error";\
}

inline std::string GetEnv(const char* pName, const char* pDefault)
{   
    ENFORCE(pName);
    char* pRes = getenv(pName);
    ENFORCE(pRes || pDefault);
    return pRes ? pRes : pDefault;
}

inline TLogLevel levelFromString(const std::string& level)
{   
    if (level == "ERROR")
        return logERROR;
    if (level == "WARNING")
        return logWARNING;
    if (level == "INFO")
        return logINFO;
    if (level == "DEBUG")
        return logDEBUG;
    if (level == "ALL")
        return logALL;
    return logINFO;
}

inline std::string nowTime()
{ 
    struct timeval tv;
    gettimeofday(&tv, 0);
    char buffer[50];
    tm r = {0};
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", localtime_r(&tv.tv_sec, &r));
    char result[100] = {0};
    sprintf(result, "%s.%06ld", buffer, (long)tv.tv_usec);
    return result;
}
