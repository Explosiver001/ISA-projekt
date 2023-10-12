#include <stdio.h>
#include <string>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ether.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

#include "pcaphandler.h"

#define ETHERNET_HEADER 14 // size of ethernet header
#define UDP_HEADER 8 // size of UDP header
#define YIADDR_OFFSET 16 // offset of YIAddr in DHCP packet
#define DHCP_OPTIONS_OFFSET 240 // 
#define OPTIONS_END 0xff
#define OPTIONS_MESSAGE_T 0x35
#define ACK 0x05


using namespace std;

PcapHandler::~PcapHandler() = default;

PcapHandler::PcapHandler(Options options, Stats stats){
    _options = options;
    _stats = stats;
    bool err = false;
    if(_options.GetFileName()){
        err = OpenOffline();
    }
    else if (_options.GetInterfaceName()){
        err = OpenLive();
    }
}

bool PcapHandler::OpenOffline(){
    char errbuf[PCAP_ERRBUF_SIZE];
    _pcap = pcap_open_offline(_options.GetFileName(), errbuf);
    if(_pcap == NULL){
        printf("%s\n",errbuf);
        exit(1);
    }
    return true; //! fix
}



bool PcapHandler::OpenLive(){
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 netp;
    bpf_u_int32 maskp;

    if(pcap_lookupnet(_options.GetInterfaceName(),&netp,&maskp,errbuf) == PCAP_ERROR)
    {
        printf("%s\n",errbuf);
        exit(1);
    }

    _pcap = pcap_open_live(_options.GetInterfaceName(), BUFSIZ, 1, 1000, errbuf);
    if(_pcap == NULL){
        printf("%s\n",errbuf);
        exit(1);
    }
    return true; //! fix
}

bool PcapHandler::CreateSetFilter(){
    string filter_expr = "( ip and udp and ( src port 67 ) )";

    bpf_u_int32 mask = 0;		    /* The netmask of our sniffing device */
    bpf_u_int32 net = 0;		    /* The IP of our sniffing device */
    if (pcap_compile(_pcap, &_fp, filter_expr.c_str(), 0, net) == -1) {
        printf("\n\npcap_compile failed\n\n");
        return false;
    }
    if (pcap_setfilter(_pcap, &_fp) == -1) {
        printf("\n\npcap_setfilter faildef\n\n");
        return false;
    }
    return true;
}

void PcapHandler::GetData(){
    struct pcap_pkthdr *header;
    const u_char *packet;
    u_int packetCount = 0;

    while (int returnValue = pcap_next_ex(_pcap, &header, &packet) >= 0)
    {
        // Show the packet number
        // fprintf(stderr, "\n\nPacket # %i\n", ++packetCount); //! delete


        struct ether_header *eptr = (struct ether_header *) packet;

        if(ntohs(eptr->ether_type) != ETHERTYPE_IP){
            // fprintf(stderr, "Ethernet type is not IP\n"); //! redo
            continue;
        }

        u_int ip_header_len;
        struct ip* ip_header = (struct ip*) (packet + ETHERNET_HEADER);
        ip_header_len = ip_header->ip_hl*4;

        if(ip_header->ip_p != IPPROTO_UDP){
            // fprintf(stderr, "IP protocol is not UDP\n"); //! redo
            continue;
        }

        struct in_addr  yip_addr = *(struct in_addr  *)(packet + ETHERNET_HEADER + ip_header_len + UDP_HEADER + YIADDR_OFFSET);

        uint32_t options_offset = ETHERNET_HEADER + ip_header_len + UDP_HEADER + DHCP_OPTIONS_OFFSET;
        uint8_t option;
        uint8_t message_type;
        while((option = *(uint8_t *)(packet + options_offset)) != OPTIONS_END && options_offset < header->caplen){
            if(option == OPTIONS_MESSAGE_T){
                message_type = *(uint8_t *)(packet + options_offset + 2);
                break;
            }
            else{
                uint8_t size = *(uint8_t *)(packet + options_offset + 1);
                options_offset = options_offset + 2 + size;
            }
        }

        // fprintf(stderr, "message_type: %d\n", message_type);
        if(message_type != ACK)
            continue;

        // fprintf(stderr, "YIAddr: %s\n", inet_ntoa(yip_addr)); //! delete

        // here collect stats
        _stats.AddIP(&yip_addr);

    }
    
    pcap_freecode(&_fp);
    pcap_close(_pcap);
}









