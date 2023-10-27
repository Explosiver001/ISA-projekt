/**
 * @file pcaphandler.h
 * @author Michal Novák (xnovak3g@stud.fit.vutbr.cz)
 * @brief Declaration of tool used for reading and extracting data from packets
 * @date 25.10.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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
    
    public:
        ~PcapHandler();
        PcapHandler(Options *options, Stats *stats, EventLogger *logger);

        /**
         * @brief Crete pcap filter for DHCP
         * 
         * @return operation result
         */
        bool CreateSetFilter();

        /**
         * @brief Open pcap file
         * 
         * @return operation result 
         */
        bool OpenOffline();

        /**
         * @brief Open live capture
         * 
         * @return operation result
         */
        bool OpenLive();

        /**
         * @brief Collects data from pcap
         * 
         */
        void CollectData();
};

#endif