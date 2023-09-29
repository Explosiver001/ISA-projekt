#include <stdarg.h>

/**
 * @brief Log information to console and syslog
 * 
 */
class EventLogger{
    private:

    public:
        EventLogger();
        void LogWarning(const char *fmt, ...);
        void LogInfo(const char *fmt, ...);
        void LogDebug(const char *fmt, ...);
        ~EventLogger();
};