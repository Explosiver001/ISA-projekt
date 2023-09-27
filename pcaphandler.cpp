#include <stdio.h>
#include <string>
#include "pcaphandler.h"

using namespace std;


PcapHandler::PcapHandler(Options options){
    _options = options;
    char errbuff[PCAP_ERRBUF_SIZE];
    // string file = "icmp.pcap";
    // string file = "dhcp-communication.pcapng";
    
    _pcap = pcap_open_offline(options.GetFileName(), errbuff);
}

bool PcapHandler::CreateSetFilter(){
    string filter_expr = "( udp and ( port 67 or port 68 ) )";
    char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
    struct bpf_program fp;		    /* The compiled filter expression */
    bpf_u_int32 mask = 0;		    /* The netmask of our sniffing device */
    bpf_u_int32 net = 0;		    /* The IP of our sniffing device */
    if (pcap_compile(_pcap, &fp, filter_expr.c_str(), 0, net) == -1) {
        return false;
    }
    if (pcap_setfilter(_pcap, &fp) == -1) {
        return(false);
    }
    return true;
}

void PcapHandler::PrintData(){
    struct pcap_pkthdr *header;
    const u_char *data;
    u_int packetCount = 0;
    while (int returnValue = pcap_next_ex(_pcap, &header, &data) >= 0)
    {
        // Show the packet number
        printf("Packet # %i\n", ++packetCount);

        // Show the size in bytes of the packet
        printf("Packet size: %d bytes\n", header->len);

        // Show a warning if the length captured is different
        if (header->len != header->caplen)
            printf("Warning! Capture size different than packet size: %ld bytes\n", header->len);

        // Show Epoch Time
        printf("Epoch Time: %d:%d seconds\n", header->ts.tv_sec, header->ts.tv_usec);

        // loop through the packet and print it as hexidecimal representations of octets
        // We also have a function that does this similarly below: PrintData()
        for (u_int i=0; (i < header->caplen ) ; i++)
        {
            // Start printing on the next after every 16 octets
            if ( (i % 16) == 0) printf("\n");

            // Print each octet as hex (x), make sure there is always two characters (.2).
            printf("%.2x ", data[i]);
        }

        // Add two lines between packets
        printf("\n\n");
    }
}






