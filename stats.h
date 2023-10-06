#ifndef _STATS_H_
#define _STATS_H_

#include <stdio.h>
#include <set>
#include <vector>
#include <cstdint>

using namespace std;

class Stats{
    private:
        typedef struct{
            uint32_t base;
            uint32_t mask;
            set<uint32_t> ip_used; 
        } StatsItem_t;

        vector<StatsItem_t> _items;

    public:
        Stats();
        Stats(std::set<char *> ip_prefixes);
        void AddIP(struct in_addr * ip);
};


#endif