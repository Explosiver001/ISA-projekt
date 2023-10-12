#ifndef _EVENT_LOGGER_H_
#define _EVENT_LOGGER_H_
#include <vector>

/**
 * @brief Log information to console and syslog
 * 
 */
class EventLogger{
    private:
        int _prefix_lines;

    public:
        EventLogger();
        void Log50Exceeded(char * base, int mask_size);
        void InitConsoleOutput(std::vector<char *> prefixes, std::vector<int> max_ips);
        void UpdateLine(int prefix_num, char* prefix, int max_devices, int devices);

        ~EventLogger();
};

#endif