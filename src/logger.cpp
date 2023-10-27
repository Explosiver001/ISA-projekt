/**
 * @file logger.cpp
 * @author Michal Novák (xnovak3g@stud.fit.vutbr.cz)
 * @brief Implementation of logging tool
 * @date 25.10.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "logger.h"
#include <syslog.h>

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


using namespace std;

EventLogger::EventLogger(){
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog ("dhcp-stats", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    initscr();
}


void EventLogger::Log50Exceeded(char * prefix){
    syslog (LOG_NOTICE, "prefix %s exceeded 50%% of allocations\n", prefix);
    refresh();
}


void EventLogger::InitConsoleOutput(std::vector<char *> prefixes, std::vector<int> max_ips){
    _prefix_lines = max_ips.size();
    printw("IP-Prefix Max-hosts Allocated addresses Utilization");
    for(int i = 0; i < prefixes.size(); i++){
        mvprintw(i+1, 0, "%s %d 0 0.0%%", prefixes.at(i), max_ips.at(i));
    }
    move(_prefix_lines+1, 0);
    refresh();
}

void EventLogger::UpdateLine(int line, char* prefix, int max_devices, int devices){
    mvprintw(line+1, 0, "%s %d %d %.2f%%", prefix, max_devices, devices, (devices/(double)max_devices)*100);
    move(_prefix_lines+1, 0);
    refresh();			
}

void EventLogger::NotifyFileEnd(){
    printw("All records from file have been processed. Press any key to exit...");
    getch();
}

EventLogger::~EventLogger(){
    endwin();
    closelog();
}