#include <stdio.h>
#include <string>
#include "pcaphandler.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define SIZE_ETHERNET 14 // size of ethernet header
#define IPV4_LENGHT 4 // 32 bits
#define IP_INHEADER_OFFSET 12 // offset in IP header from src ip

using namespace std;


#include "stats.h"


PcapHandler::PcapHandler(Options options){
    _options = options;
    char errbuff[PCAP_ERRBUF_SIZE];
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

    Stats stats(_options.GetIPPrefixes());

    while (int returnValue = pcap_next_ex(_pcap, &header, &data) >= 0)
    {
        // Show the packet number
        printf("Packet # %i\n", ++packetCount);

        // Show the size in bytes of the packet
        // printf("Packet size: %d bytes\n", header->len);

        // Show a warning if the length captured is different
        if (header->len != header->caplen)
            printf("Warning! Capture size different than packet size: %ld bytes\n", header->len);

        u_char ipv = (*(u_char *)(data+SIZE_ETHERNET)) >> 4;

        struct in_addr src = *(struct in_addr*)(data+SIZE_ETHERNET+IP_INHEADER_OFFSET);
        struct in_addr dst = *(struct in_addr*)(data+SIZE_ETHERNET+IP_INHEADER_OFFSET+IPV4_LENGHT);

        printf("IPV: %d\n", ipv);
        printf("SRC: %s, %x\n", inet_ntoa(src), ntohl(src.s_addr));
        stats.AddIP(ntohl(src.s_addr));
        uint32_t a = ntohl(dst.s_addr);
        printf("DST: %s, %x\n", inet_ntoa(dst), a);
        stats.AddIP(a);

        // Add two lines between packets
        printf("\n\n");
    }
}






