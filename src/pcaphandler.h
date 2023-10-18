#ifndef _PCAP_HANDLER_H_
#define _PCAP_HANDLER_H_

#include <pcap.h>
#include "optparser.h"
#include "stats.h"
#include "logger.h"

class PcapHandler{
    private:
        Options _options;
        Stats _stats;
        EventLogger _logger;
        pcap_t * _pcap;
        struct bpf_program _fp;
        bool OpenOffline();
        bool OpenLive();
    
    public:
        ~PcapHandler();
        PcapHandler(Options options, Stats stats, EventLogger logger);
        bool CreateSetFilter();
        void CollectData();
};

#endif