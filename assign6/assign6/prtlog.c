#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "pcap.h"
#include "dnet.h"
#include "prtlog.h"

void printTime(struct my_pkthdr p);

int main (int argc, char *argv[]) {
    int fdin;
    int pnum = 0;
    struct pcap_file_header fheader;
    struct timev time;
    struct my_pkthdr pheader;
    char buf[4096];
    int firsttime;
    unsigned int b_sec, c_sec;
    int b_usec, c_usec;
    struct eth_hdr *ehdr;
    struct arp_hdr *arp;
    struct ip_hdr *ip;
    char *etype, *ip_prot, *arp_prot;

    /* Check usage */
    if ( argc != 2 ) {
        fprintf(stderr,"Usage: logprt <file>\n");
        exit(-1);
    }
    
    /* Open file or die */
    if((fdin = open(argv[1], O_RDONLY)) < 0 ) {
    perror(argv[1]);
    exit(-1);
    }
    
    int n = read(fdin, &fheader, sizeof(struct pcap_file_header));

    if (n != sizeof(struct pcap_file_header)) {
        close(fdin);
        perror("Error Reading pcap file header");
        exit(-1);
        }
    //printf("Read %d bytes\n", n);                               
    printFileType(fheader.magic);
    printf("Version major number = %d\n", fheader.version_major);
    printf("Version minor number = %d\n", fheader.version_minor);
    printf("GMT to local correction = %d\n", fheader.thiszone);
    printf("Timestamp accuracy = %d\n", fheader.sigfigs);
    printf("Snaplen = %u\n", fheader.snaplen);
    printf("Linktype = %d\n\n", fheader.linktype);

    while( (n = read(fdin, &pheader, sizeof(struct my_pkthdr))) == sizeof(struct my_pkthdr)){

        int m, n;
        if( (m = read(fdin, buf, pheader.len)) != pheader.len){
            printf("read err\n");
            exit(-1);
        }
        

        ehdr = (struct eth_hdr *)buf;
        
        
        if(ntohs(ehdr->eth_type) == ETH_TYPE_IP){
            etype = "IP";
            ip = (struct ip_hdr *)(buf+sizeof(struct eth_hdr));
            if(ip->ip_p == 1)
                ip_prot = "ICMP";
             else if(ip->ip_p == 2)
                 ip_prot = "IGMP";
             else if(ip->ip_p == 6)
                 ip_prot = "TCP";
             else if(ip->ip_p == 17)
                 ip_prot = "UDP";
             else ip_prot = "UNRECOGNIZED";
        }else if(ntohs(ehdr->eth_type) == ETH_TYPE_ARP){
            etype = "ARP";
            arp = (struct arp_hdr *)(buf+sizeof(struct eth_hdr));
            arp->ar_op = ntohs(arp->ar_op);
            if(arp->ar_op == 1)
                arp_prot = "Arp Request";
            else if(arp->ar_op == 2)
                arp_prot = "Arp Reply";
            else if(arp->ar_op == 3)
                arp_prot = "Arp Revrequest";
            else if(arp->ar_op == 4)
                arp_prot = "Arp Revreply";
            else arp_prot = "UNRECOGNIZED";
        }else{
            switch(ntohs(ehdr->eth_type)){
                case ETH_TYPE_PUP:
                    etype = "PUP";
                    break;
                case ETH_TYPE_REVARP:
                    etype = "REVARP";
                    break;
                case ETH_TYPE_8021Q:
                    etype = "8021Q";
                    break;
                case ETH_TYPE_IPV6:
                    etype = "IPV6";
                    break;
                case ETH_TYPE_MPLS:
                    etype = "MPLS";
                    break;
                case ETH_TYPE_MPLS_MCAST:
                    etype = "MPLS_MCAST";
                    break;
                case ETH_TYPE_PPPOEDISC:
                    etype = "PPPOEDISC";
                    break;
                case ETH_TYPE_PPPOE:
                    etype = "PPPOE";
                    break;
                case ETH_TYPE_LOOPBACK:
                    etype = "LOOPBACK";
                    break;
                default:
                    etype = "UNRECOGNIZED";
            }
            lseek(fdin, (pheader.len-m), SEEK_CUR);
        }

        printf("Packet %d\n", pnum++);
        printTime(pheader);
        printf("Captured Packet Length = %d\n", pheader.caplen);
        printf("Actual Packet Length = %d\n", pheader.len);
        printf("Ethernet Header\n");
        printf("   %s\n", etype);
        if (etype == "IP")
            printf("      %s\n", ip_prot);
        else if (etype == "ARP")
            printf("      arp operation = %s\n", arp_prot);
        printf("\n");

    }

    close(fdin);
    exit(0);
}



/* Prints the type of file */
void printFileType(u_int32_t magic) {
    if (magic == PCAP_MAGIC)
        printf("PCAP_MAGIC\n");
    else if(magic == PCAP_SWAPPED_MAGIC)
        printf("PCAP_SWAPPED_MAGIC\n");
    else if(magic == PCAP_MODIFIED_MAGIC)
        printf("PCAP_MODIFIED_MAGIC\n");
    else if(magic == PCAP_SWAPPED_MODIFIED_MAGIC)
        printf("PCAP_SWAPPED_MODIFIED_MAGIC\n");
    else
        printf("UNKNOWN FILE TYPE");
}

/* Prints the packet time */
void printTime(struct my_pkthdr p){
    static int firsttime = 1;
    static unsigned int b_sec;
    static int b_usec;
                    
    unsigned int c_sec;
    int c_usec;
                                
    if(firsttime) {
        firsttime = 0;
        b_sec = p.ts.tv_sec;
        b_usec = p.ts.tv_usec;
    }
                                                                        
    c_sec = (unsigned)p.ts.tv_sec - b_sec;
    c_usec = (unsigned)p.ts.tv_usec - b_usec;
                                                                                    
    while(c_usec < 0) {
        c_usec += 1000000;
        c_sec--;
    }                                                                                                                
    printf("%05u.%06u\n", (unsigned)c_sec, (unsigned)c_usec);
}
