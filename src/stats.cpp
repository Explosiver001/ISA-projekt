/**
 * @file stats.cpp
 * @author Michal Novák (xnovak3g@stud.fit.vutbr.cz)
 * @brief Implementation of statistics manager
 * @date 25.10.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "stats.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <algorithm>

using namespace std;

Stats::~Stats() = default;

Stats::Stats() = default;


Stats::Stats(std::set<char *> ip_prefixes, EventLogger logger){
    _logger = logger;
    for(set<char *>::iterator it = ip_prefixes.begin(); it != ip_prefixes.end(); it++){
        string prefix(*it);
        StatsItem_t item;
        struct sockaddr_in addr;

        inet_aton(prefix.substr(0,prefix.find('/')).c_str(), &addr.sin_addr);
        item.prefix = *it;
        item.warn = false;
        item.mask_len = atoi(prefix.substr(prefix.find('/')+1).c_str());
        item.mask = item.mask_len ? ~0 << 32 - item.mask_len : 0;
        item.network_ip = ntohl(addr.sin_addr.s_addr) & item.mask;
        item.broadcast_ip = item.network_ip | ~item.mask; 
        item.max_devices = (~item.mask - 1);
        _items.push_back(item);
    }
    sort(_items.begin(), _items.end(), [](StatsItem_t const &a, StatsItem_t const &b) {return a.mask_len < b.mask_len;});
}

void Stats::AddIP(struct in_addr * ip){
    uint32_t normalized_ip = ntohl(ip->s_addr);
    for(int i = 0; i < _items.size(); i++){
        StatsItem_t * item = &_items.at(i);
        if(!BelongsToSubnet(item, normalized_ip))
            continue;
        uint32_t network_ip = htonl(item->network_ip);

        item->ip_used.insert(normalized_ip);
        _logger.UpdateLine(i, item->prefix, item->max_devices, item->ip_used.size());
        if(!(item->ip_used.size() >= (~item->mask - 1) / 2.0) || item->warn){
            continue;
        }

        item->warn = true;
        _logger.Log50Exceeded(item->prefix);
    }
}   

bool Stats::BelongsToSubnet(StatsItem_t * subnet, uint32_t ip){
    return ((subnet->network_ip == (ip & subnet->mask)) && (subnet->network_ip != ip) && (subnet->broadcast_ip != ip));
}

void Stats::InitConsole(){
    std::vector<char *> prefixes;
    std::vector<uint32_t> max_ips;
    for(int i = 0; i < _items.size(); i++){
        StatsItem_t * item = &_items.at(i);
        prefixes.push_back(item->prefix);
        max_ips.push_back(item->max_devices);
    }

    _logger.InitConsoleOutput(prefixes, max_ips);
}