#ifndef _PCAP_HANDLER_H_
#define _PCAP_HANDLER_H_

#include <pcap.h>
#include "optparser.h"
#include "stats.h"

class PcapHandler{
    private:
        Options _options;
        Stats _stats;
        pcap_t * _pcap;
        bool OpenOffline();
        bool OpenLive();
    
    public:
        PcapHandler(Options options, Stats stats);
        bool CreateSetFilter();
        void GetData();
};

#endif