/**
 * @file pcaphandler.cpp
 * @author Michal Novák (xnovak3g@stud.fit.vutbr.cz)
 * @brief Implementation of tool used for reading and extracting data from packets
 * @date 25.10.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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
#define DHCP_OPTIONS_OFFSET 240 // DHCP start of options part
#define OPTIONS_END 0xff // DHCP options end
#define OPTIONS_MESSAGE_T 0x35 // DHCP message type option 
#define ACK 0x05 // DHCP ACK message


using namespace std;

PcapHandler::~PcapHandler() = default;

PcapHandler::PcapHandler(Options *options, Stats *stats, EventLogger *logger){
    _options = *options;
    _stats = *stats;
    _logger = *logger;
    _stats.InitConsole();
}

bool PcapHandler::OpenOffline(){
    char errbuf[PCAP_ERRBUF_SIZE];
    _pcap = pcap_open_offline(_options.GetFileName(), errbuf);
    if(_pcap == NULL){
        fprintf(stderr, "%s\n",errbuf);
        return false;
    }
    return true;
}

bool PcapHandler::OpenLive(){
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 netp;
    bpf_u_int32 maskp;

    if(pcap_lookupnet(_options.GetInterfaceName(),&netp,&maskp,errbuf) == PCAP_ERROR)
    {
        fprintf(stderr, "%s\n",errbuf);
        return false;
    }

    _pcap = pcap_open_live(_options.GetInterfaceName(), BUFSIZ, 1, 1000, errbuf);
    if(_pcap == NULL){
        fprintf(stderr, "%s\n",errbuf);
        return false;
    }
    return true;
}

bool PcapHandler::CreateSetFilter(){
    string filter_expr = "( ip and udp and ( src port 67 ) )";

    bpf_u_int32 net = 0;		    /* The IP of our sniffing device */
    if (pcap_compile(_pcap, &_fp, filter_expr.c_str(), 0, net) == -1) {
        fprintf(stderr, "\n\npcap_compile failed\n\n");
        return false;
    }
    if (pcap_setfilter(_pcap, &_fp) == -1) {
        fprintf(stderr, "\n\npcap_setfilter failed\n\n");
        return false;
    }
    return true;
}

void PcapHandler::FreePcap(){
    if(&_fp)
        pcap_freecode(&_fp);
    if(_pcap)
        pcap_close(_pcap);
}

void PcapHandler::CollectData(){
    struct pcap_pkthdr *header; // packet header
    const u_char *packet; // data in packet

    while (int returnValue = pcap_next_ex(_pcap, &header, &packet) >= 0)
    {
        struct ether_header *eptr = (struct ether_header *) packet; //cast to ethertnet header

        if(ntohs(eptr->ether_type) != ETHERTYPE_IP){ // packet does not conatin IPv4
            continue;
        }

        u_int ip_header_len;
        struct ip* ip_header = (struct ip*) (packet + ETHERNET_HEADER);
        ip_header_len = ip_header->ip_hl*4;

        if(ip_header->ip_p != IPPROTO_UDP){ // packet does not contain UDP datagram
            continue;
        }

        // get assigned client IP
        struct in_addr  yip_addr = *(struct in_addr  *)(packet + ETHERNET_HEADER + ip_header_len + UDP_HEADER + YIADDR_OFFSET);

        // count options offset from start of packet
        uint32_t options_offset = ETHERNET_HEADER + ip_header_len + UDP_HEADER + DHCP_OPTIONS_OFFSET;
        uint8_t option; // prepare for option
        uint8_t option_data; 

        // get option end quit if MessageType or find another option 
        while((option = *(uint8_t *)(packet + options_offset)) != OPTIONS_END && options_offset < header->caplen){
            if(option == OPTIONS_MESSAGE_T){
                option_data = *(uint8_t *)(packet + options_offset + 2);
                break;
            }
            else{
                uint8_t size = *(uint8_t *)(packet + options_offset + 1);
                options_offset = options_offset + 2 + size;
            }
        }

        // check if type is ACK
        if(option_data != ACK)
            continue;

        // add client ip to stats
        _stats.AddIP(&yip_addr);

    }
    // free resources
    FreePcap();

    // if using file, pause output until any key is pressed
    if(_options.GetFileName())
        _logger.NotifyFileEnd();

}









