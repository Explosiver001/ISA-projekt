from scapy.all import *
import ipaddress
import random

def main():

    if len(sys.argv)<3:
        print("run: sudo python3 generate.py <interface> <prefix>")
        sys.exit(1)
    else:
        tap_interface = sys.argv[1]
        prefix = sys.argv[2]

    all_ips = [str(ip) for ip in ipaddress.IPv4Network(prefix)]

    src_ip = all_ips[0]
    
    all_ips = all_ips[1:-1]
    random.shuffle(all_ips)
    random.seed(0)
    for yiaddr in all_ips:
        ethernet = Ether(dst='ff:ff:ff:ff:ff:ff',src="00:00:00:00:00:00",type=0x800)
        ip = IP(src = src_ip,dst='255.255.255.255')
        udp =UDP (sport=67,dport=68)
        _,hw = get_if_raw_hwaddr(tap_interface)
        bootp = BOOTP(chaddr = hw, yiaddr = yiaddr,xid =  0x01020304,flags= 1)
        dhcp = DHCP(options=[
                        ("server_id", src_ip),
                        ('param_req_list',
                        [
                        int(scapy.all.DHCPRevOptions["subnet_mask"][0]),
                        int(scapy.all.DHCPRevOptions["router"][0]),
                        int(scapy.all.DHCPRevOptions["name_server"][0])
                        ]),
                        ("message-type","ack" if random.randint(0,1) else "offer"),
                        "end"])
        packet = ethernet / ip / udp / bootp / dhcp

        sendp(packet, iface = tap_interface)
        time.sleep(0.5)


if __name__ == '__main__':
    main()