#ifndef _STATS_H_
#define _STATS_H_

#include <stdio.h>
#include <set>
#include <vector>
#include <cstdint>
#include "logger.h"

using namespace std;

class Stats{
    private:
        EventLogger _logger;

        typedef struct{
            char * prefix;
            uint32_t network_ip;
            uint32_t mask;
            uint8_t mask_len;
            bool warn;
            set<uint32_t> ip_used; 
        } StatsItem_t;
        vector<StatsItem_t> _items;

        void InitConsole();

        
    public:
        ~Stats();
        Stats();
        Stats(std::set<char *> ip_prefixes, EventLogger logger);
        void AddIP(struct in_addr * ip);
};


#endif