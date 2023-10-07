#include "optparser.h"
#include "pcaphandler.h"
#include "logger.h"
#include "stats.h"

int main(int argc, char *argv[])
{
    EventLogger logger;
    Options options(argc,argv);
    Stats stats(options.GetIPPrefixes(), logger);
    PcapHandler pcaphandler(options, stats);
    pcaphandler.CreateSetFilter();
    pcaphandler.GetData();

    return 0;
}