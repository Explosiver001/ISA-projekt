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
    // printf("prefix %s/%u exceeded 50%% of allocations\n", base, mask_size);
}

void EventLogger::InitConsoleOutput(std::vector<char *> prefixes, std::vector<int> max_ips){
    _prefix_lines = max_ips.size();
    printf("IP-Prefix Max-hosts Allocated addresses Utilization\n");
    for(int i = 0; i < prefixes.size(); i++){
        printf("%s %d 0.0%%\n", prefixes.at(i), max_ips.at(i));
    }
}

void EventLogger::UpdateLine(int line, char* prefix, int max_devices, int devices){
    int shift = _prefix_lines - line;
    for(int i = 0; i < shift; i++){
        printf("\x1b[1F"); // Move to beginning of previous line
    }
    printf("\x1b[1K"); // Erase line
    printf("%s %d %.2f%\n", prefix, max_devices, (devices/(double)max_devices)*100);
    for(int i = 0; i < shift; i++){
        printf("\x1b[1E"); // Move to beginning of next line
    }
}

EventLogger::~EventLogger(){
    closelog();
}