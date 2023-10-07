#ifndef _EVENT_LOGGER_H_
#define _EVENT_LOGGER_H_

/**
 * @brief Log information to console and syslog
 * 
 */
class EventLogger{
    private:

    public:
        EventLogger();
        void Log50Exceeded(char * base, int mask_size);

        ~EventLogger();
};

#endif