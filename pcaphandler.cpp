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
#define YIADDR_OFFSET 16 // offset of YIAddr in DHCP packet

using namespace std;


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
    else{
        //error
        exit(1);
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

void PcapHandler::GetData(){
    struct pcap_pkthdr *header;
    const u_char *packet;
    u_int packetCount = 0;

    while (int returnValue = pcap_next_ex(_pcap, &header, &packet) >= 0)
    {
        // Show the packet number
        printf("Packet # %i\n", ++packetCount); //! delete


        struct ether_header *eptr = (struct ether_header *) packet;

        if(ntohs(eptr->ether_type) != ETHERTYPE_IP){
            printf("Ethernet type is not IP\n"); //! redo
            continue;
        }

        u_int ip_header_len;
        struct ip* ip_header = (struct ip*) (packet + ETHERNET_HEADER);
        ip_header_len = ip_header->ip_hl*4;

        if(ip_header->ip_p != IPPROTO_UDP){
            printf("IP protocol is not UDP\n"); //! redo
            continue;
        }

        const struct udphdr *udp_header = (const struct udphdr*) (packet + ETHERNET_HEADER + ip_header_len);

        struct in_addr  yip_addr = *(struct in_addr  *)(packet + ETHERNET_HEADER + ip_header_len + sizeof(udp_header) + YIADDR_OFFSET);
        printf("YIAddr: %s\n", inet_ntoa(yip_addr)); //! delete

        // here collect stats
        _stats.AddIP(&yip_addr);

    }
}







