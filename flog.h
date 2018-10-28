#pragma once

#include <string>
#include <sys/time.h>
#include <sstream>
#include <stdexcept>

enum TFlogLevel {flogERROR, flogWARNING, flogINFO, flogDEBUG, flogDEBUG1, flogDEBUG2, flogALL};
namespace cpputil {

    inline std::string NowTime()
    {
        struct timeval tv;
        gettimeofday(&tv, 0);
        char buffer[11];
        tm r;
        strftime(buffer, sizeof(buffer), "%X", localtime_r(&tv.tv_sec, &r));
        char result[100] = {0};
        std::sprintf(result, "%s.%06ld", buffer, (long)tv.tv_usec);
        return result;
    }
   inline TFlogLevel FromString(const std::string& level)
    {
        if (level == "ERROR")
            return flogERROR;
        if (level == "WARNING")
            return flogWARNING;
        if (level == "INFO")
            return flogINFO;
        if (level == "DEBUG")
            return flogDEBUG;
        if (level == "DEBUG1")
            return flogDEBUG1;
        if (level == "DEBUG2")
            return flogDEBUG2;
        if (level == "ALL")
            return flogALL;
        return flogINFO;
    }
    inline const char* LogToString(TFlogLevel level)
    {
        static const char* buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "ALL"};
        return buffer[level];
    }
    inline std::string GetEnv(const char* pName, const char* pDefault)
    {
        if (!pName)
            throw std::runtime_error("Please specify an environment value!");
        char* pRes = getenv(pName);
        if (!pRes)
        {
            if (!pDefault)
                throw std::runtime_error("Please specify a default for missing '" + std::string(pName)+ "' environment variable!");
            return pDefault;
        }
        return pRes;
    }
    TFlogLevel& ReportingLevel()
    {
        static TFlogLevel reportingLevel = FromString(GetEnv("FLOG_LEVEL", "INFO"));
        return reportingLevel;
    }
    inline FILE*& LogStream()
    {
        static FILE* pStream = stderr;
        return pStream;
    }
    unsigned long threadID()
    {
        static unsigned long firstThreadID = pthread_self();
        unsigned long id = pthread_self();
        return (id == firstThreadID) ? 0 : id;
    }
    struct LogLine {
        std::ostringstream o;
        TFlogLevel level;
        std::ostream& get(TFlogLevel l) {
            level = l;
            return o;
        }
        ~LogLine() {
            std::ostringstream line;
            line << NowTime() << " [" << threadID() << "]";
            line << " " << LogToString(level) << ": ";
            line << std::string(level <= flogDEBUG ? 0 : level - flogDEBUG, '\t');
            line << o.str();
            line << "\n";
            fprintf(LogStream(), "%s", line.str().c_str());
            fflush(LogStream());
        }
    };
}

#define FLOG(lineLevel) if (flog##lineLevel > cpputil::ReportingLevel()); else cpputil::LogLine().get(flog##lineLevel)
#define STD_FUNCTION_BEGIN try {
#define STD_FUNCTION_END }                      \
        catch (const std::exception& e)         \
        {                                       \
            FLOG(ERROR) << e.what();     \
        }                                       \
        catch (...)                             \
        {                                       \
            FLOG(ERROR) << "Unknown error"; \
        }


