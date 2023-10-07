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

void EventLogger::Log50Exceeded(char * base, int mask_size){
    // syslog (LOG_INFO, "prefix %s/%d exceeded 50%% of allocations\n", base, mask_size);
    printf("prefix %s/%u exceeded 50%% of allocations\n", base, mask_size);
}

EventLogger::~EventLogger(){
    closelog ();
}