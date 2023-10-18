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
        fprintf(stderr, "%s\n", e.what());
        fprintf(stderr, "Use -h to see all options.\n");
        FreeMem(logger, NULL, NULL);
        exit(1);
    }

    if(options->PrintHelp()){
        printf("HELP\n");
        FreeMem(logger, options, NULL);
        exit(0);
    }
    Stats *stats = new Stats(options->GetIPPrefixes(), *logger);
    PcapHandler pcaphandler(*options, *stats, *logger);
    pcaphandler.CreateSetFilter();
    pcaphandler.CollectData();
    FreeMem(logger, options, stats);
    return 0;
}