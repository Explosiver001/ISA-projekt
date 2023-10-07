#include "stats.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <algorithm>

using namespace std;

Stats::Stats() = default;

Stats::Stats(std::set<char *> ip_prefixes, EventLogger logger){
    _logger = logger;
    for(set<char *>::iterator it = ip_prefixes.begin(); it != ip_prefixes.end(); it++){
        string prefix(*it);
        StatsItem_t item;
        struct sockaddr_in addr;

        inet_aton(prefix.substr(0,prefix.find('/')).c_str(), &addr.sin_addr);
        item.mask_len = atoi(prefix.substr(prefix.find('/')+1).c_str());
        item.mask = item.mask_len ? ~0 << 32 - item.mask_len : 0;
        item.network_ip = ntohl(addr.sin_addr.s_addr) & item.mask;
        // printf("ip: %x\nmask: %x\n\n", item.network_ip, item.mask);
        _items.push_back(item);
    }
    sort(_items.begin(), _items.end(), [](StatsItem_t const &a, StatsItem_t const &b) {return a.mask_len < b.mask_len;});
}

void Stats::AddIP(struct in_addr * ip){
    uint32_t normalized_ip = ntohl(ip->s_addr);
    for(int i = 0; i < _items.size(); i++){
        StatsItem_t * item = &_items.at(i);
        if(item->network_ip == (normalized_ip & item->mask)){
            printf("ip: %s", inet_ntoa(*ip));
            uint32_t network_ip = htonl(item->network_ip);
            printf(" under base: %s/%d\n", inet_ntoa(*(struct in_addr*)&network_ip), item->mask_len);
            if(item->ip_used.find(normalized_ip) == item->ip_used.end()){
                item->ip_used.insert(normalized_ip);
                if(item->ip_used.size() >= (~item->mask - 2) / 2.0){
                    _logger.Log50Exceeded(inet_ntoa(*(struct in_addr*)&network_ip), item->mask_len);
                }
            }
        }
    }
}   