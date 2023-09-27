#ifndef _PCAP_HANDLER_H_
#define _PCAP_HANDLER_H_

#include <pcap.h>
#include "optparser.h"

class PcapHandler{
    private:
        Options _options;
        pcap_t * _pcap;
    
    public:
        PcapHandler(Options options);
        bool CreateSetFilter();
        void PrintData();
};

#endif