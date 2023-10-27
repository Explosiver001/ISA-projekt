/**
 * @file stats.h
 * @author Michal Novák (xnovak3g@stud.fit.vutbr.cz)
 * @brief Declaration of statistics manager
 * @date 25.10.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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
        /**
         * @brief Subnet information
         * 
         */
        typedef struct{
            char * prefix;
            uint32_t network_ip;
            uint32_t broadcast_ip;
            uint32_t mask;
            uint32_t max_devices;
            uint8_t mask_len;
            bool warn;
            set<uint32_t> ip_used; 
        } StatsItem_t;
        vector<StatsItem_t> _items;

        /**
         * @brief Decides if IP is part of subnet
         * 
         * @param subnet 
         * @param ip 
         */
        bool BelongsToSubnet(StatsItem_t * subnet, uint32_t ip);

        
    public:
        ~Stats();
        Stats();
        Stats(std::set<char *> ip_prefixes, EventLogger logger);

        /**
         * @brief Calls logger, that initializes console output
         * 
         */
        void InitConsole();

        /**
         * @brief Marks ip as taken, if belongs to subnet
         * 
         * @param ip 
         */
        void AddIP(struct in_addr * ip);
};


#endif