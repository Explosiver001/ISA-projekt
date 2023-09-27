#include "optparser.h"
#include "pcaphandler.h"


int main(int argc, char *argv[])
{
    Options options(argc,argv);
    PcapHandler pcaphandler(options);
    pcaphandler.CreateSetFilter();
    pcaphandler.PrintData();
}