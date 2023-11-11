/**
 * @file main.cpp
 * @author Michal Novák (xnovak3g@stud.fit.vutbr.cz)
 * @brief Program entry point
 * @date 25.10.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "optparser.h"
#include "pcaphandler.h"
#include "logger.h"
#include "stats.h"
#include <stdexcept>
#include <csignal>

/**
 * @brief Delete all dynamic object memory
 * 
 * @param logger Eventlogger
 * @param options Options
 * @param stats Stats
 */
void FreeMem(EventLogger *logger, Options *options, Stats *stats){
    if(logger)
        delete logger;
    if(options)
        delete options;
    if(stats)
        delete stats;
}


int main(int argc, char *argv[])
{
    EventLogger *logger = new EventLogger(); // init EventLogger
    signal(SIGINT, [](int signum) {});
    Options *options = NULL;
    Stats *stats = NULL;
    try{
        options = new Options(argc,argv); // init Options
        stats = new Stats(options->GetIPPrefixes(), *logger); // init stats
    }
    catch(std::invalid_argument& e){ // Options could not create
        FreeMem(logger, options, stats);
        fprintf(stderr, "%s\n", e.what());
        fprintf(stderr, "Use -h to see all options.\n");
        exit(1);
    }

    // program was launched with -h
    if(options->PrintHelp()){
        FreeMem(logger, options, NULL);
        printf("./dhcp-stats [-r <filename>] [-i <interface-name>] <ip-prefix> [ <ip-prefix> [ ... ] ]\n");
        printf("\t-r <filename>\t- statistics will be collected from pcap file\n");
        printf("\t-i <interface>\t- interface that will capture traffic\n");
        printf("\t<ip-prefix>\t- network prefixes that will be included into statisctics\n");
        exit(0);
    }


    PcapHandler pcaphandler(options, stats, logger); // init pcap handler

    bool err = false;
    if(options->GetFileName()){
        err = !pcaphandler.OpenOffline();
    }
    else if (options->GetInterfaceName()){
        err = !pcaphandler.OpenLive();
    }
    if(err){
        fprintf(stderr, "Use -h to see all options.\n");
        exit(1);
    }

    if(!pcaphandler.CreateSetFilter()){
        FreeMem(logger, options, stats);
        fprintf(stderr, "Could not create or set pcap filter!\n");
        exit(1);
    }

    pcaphandler.CollectData();

    FreeMem(logger, options, stats);
    return 0;
}
