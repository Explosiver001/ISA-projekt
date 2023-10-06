#include "stats.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

Stats::Stats() = default;

Stats::Stats(std::set<char *> ip_prefixes){
    for(set<char *>::iterator it = ip_prefixes.begin(); it != ip_prefixes.end(); it++){
        string prefix(*it);
        StatsItem_t item;
        struct sockaddr_in addr;

        inet_aton(prefix.substr(0,prefix.find('/')).c_str(), &addr.sin_addr);
        int mask_len = atoi(prefix.substr(prefix.find('/')+1).c_str());
        item.mask = mask_len ? ~0 << 32 - mask_len : 0;
        item.base = ntohl(addr.sin_addr.s_addr) & item.mask;
        printf("ip: %x\nmask: %x\n\n", item.base, item.mask);
        _items.push_back(item);
    }
}

void Stats::AddIP(struct in_addr * ip){
    uint32_t normalized_ip = ntohl(ip->s_addr);
    for(int i = 0; i < _items.size(); i++){
        StatsItem_t * item = &_items.at(i);
        if(item->base == (normalized_ip & item->mask)){
            printf("ip: %s", inet_ntoa(*ip));
            uint32_t base = htonl(item->base);
            printf(" under base: %s\n", inet_ntoa(*(struct in_addr*)&base));
            item->ip_used.insert(normalized_ip);
            if(item->ip_used.size() >= ~item->mask - 2){
                // notify 50%
            }
        }
    }
}   