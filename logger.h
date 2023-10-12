#ifndef _EVENT_LOGGER_H_
#define _EVENT_LOGGER_H_
#include <vector>
#include <ncurses.h>

/**
 * @brief Log information to console and syslog
 * 
 */
class EventLogger{
    private:

    public:
        EventLogger();
        void Log50Exceeded(char * base, int mask_size);
        void InitConsoleOutput(std::vector<char *> prefixes, std::vector<int> max_ips);
        void UpdateLine(int line, char* prefix, double percentage);

        ~EventLogger();
};

#endif