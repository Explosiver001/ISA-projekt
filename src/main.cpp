#include "optparser.h"
#include "pcaphandler.h"
#include "logger.h"
#include "stats.h"
#include <stdexcept>

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
    EventLogger *logger = new EventLogger();
    Options *options;
    try{
        options = new Options(argc,argv);
    }
    catch(std::invalid_argument& e){
        FreeMem(logger, NULL, NULL);
        fprintf(stderr, "%s\n", e.what());
        fprintf(stderr, "Use -h to see all options.\n");
        exit(1);
    }

    if(options->PrintHelp()){
        FreeMem(logger, options, NULL);
        printf("HELP\n");
        exit(0);
    }
    Stats *stats = new Stats(options->GetIPPrefixes(), *logger);
    PcapHandler pcaphandler(*options, *stats, *logger);
    if(!pcaphandler.CreateSetFilter()){
        FreeMem(logger, options, stats);
        fprintf(stderr, "Could not create or set pcap filter!\n");
        exit(1);
    }
    pcaphandler.CollectData();
    FreeMem(logger, options, stats);
    return 0;
}