#include "logger.h"
#include <syslog.h>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>


using namespace std;

/**
 * @brief Construct a new Event Logger:: Event Logger object
 * 
 */
EventLogger::EventLogger(){
    setlogmask (LOG_UPTO (LOG_NOTICE));
    time_t now = time(0);
    char time_str[50];
    tm * curr_time;
    curr_time = localtime(&now);
    strftime(time_str, 50, "dhcap_stats_log_%Y_%m_%d", curr_time);
    openlog (time_str, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

/**
 * @brief Log warning level event
 * 
 * @param fmt formatted string
 * @param ... arguments
 */
void EventLogger::LogWarning(const char *fmt, ...){
    va_list args;
	va_start(args, fmt);
    syslog (LOG_WARNING, fmt, args);
    fprintf(stderr, "WARN:\t");
    fprintf(stderr, fmt, args);
    va_end(args);
}

/**
 * @brief Log information level event
 * 
 * @param fmt formatted string
 * @param ... arguments
 */
void EventLogger::LogInfo(const char *fmt, ...){
    va_list args;
	va_start(args, fmt);
    syslog (LOG_INFO, fmt, args);
    fprintf(stderr, "INFO:\t");
    fprintf(stderr, fmt, args);
    va_end(args);
}

/**
 * @brief Log debug level event
 * 
 * @param fmt formatted string
 * @param ... arguments
 */
void EventLogger::LogDebug(const char *fmt, ...){
    va_list args;
	va_start(args, fmt);
    syslog (LOG_DEBUG, fmt, args);
    fprintf(stderr, "DEBUG:\t");
    fprintf(stderr, fmt, args);
    va_end(args);
}

EventLogger::~EventLogger(){
    closelog ();
}