#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/time.h>
#include <sys/fcntl.h>
#include "pcap.h"
#include "dnet.h"
#include "prtlog.h"

int main(int argc, char *argv[]) {

  // Allocate memory for file header
  pcap_file_header *file_header;
  my_pkthdr *pkthdr;
  eth_hdr *ehdr;
  arp_hdr *ahdr;
  ip_hdr *ihdr;
  int i, bytes;
  char *buffer;

  if(argc < 2) {
    print_usage(argv[0]);
  }

  // Open tcpdump file
  int fdin = open(argv[1], O_RDONLY);
  if(fdin == -1) {
    perror(argv[1]);
    exit(-1);
  }

  // Allocate memory for headers
  file_header = (pcap_file_header *) malloc(sizeof(pcap_file_header));
  pkthdr = (my_pkthdr *) malloc(sizeof(my_pkthdr));
  if(file_header == NULL || pkthdr == NULL) {
    perror("malloc");
    close(fdin);
    exit(-1);
  }

  // Read file header
  read_struct(fdin, file_header, sizeof(pcap_file_header)); 

  // Print file header  
  pcap_file_header_print(file_header);
  printf("\n");

  // Read packets
  i = 0;
  while(read_struct(fdin, pkthdr, sizeof(my_pkthdr))) {
    my_pkthdr_print(pkthdr, i++);

    // Read the rest of the packet
    int packlen = pkthdr->caplen;

    // Allocate enough memory for the packet
    buffer = (char *) malloc(packlen);
    if(buffer == NULL) {
      perror("malloc");
      close(fdin);
      exit(-1);
    }

    read_struct(fdin, buffer, packlen);

    packlen -= sizeof(eth_hdr);
    ehdr = (eth_hdr *) (buffer);
    eth_hdr_print(ehdr);

    if(ntohs(ehdr->eth_type) == ETH_TYPE_ARP) {
      packlen -= sizeof(struct arp_hdr);
      ahdr = (arp_hdr *)(buffer + sizeof(eth_hdr));
      arp_hdr_print(ahdr);
    } 

    else if(ntohs(ehdr->eth_type) == ETH_TYPE_IP) {
      packlen -= sizeof(ip_hdr);
      ihdr = (ip_hdr *)(buffer + sizeof(eth_hdr));
      ip_hdr_print(ihdr);
    } 
    printf("\n");
    free(buffer);
  }
  
  // Free structs
  free(file_header);
  free(pkthdr);

  close(fdin);
  return 0;
}


void pcap_file_header_print(pcap_file_header *header) {
  switch(header->magic) {
    case PCAP_MAGIC:
      printf("PCAP_MAGIC\n");
      break;
    case PCAP_SWAPPED_MAGIC:
      printf("PCAP_SWAPPED_MAGIC\n");
      break;
    case PCAP_MODIFIED_MAGIC:
      printf("PCAP_MODIFIED_MAGIC\n");
      break;
    case PCAP_SWAPPED_MODIFIED_MAGIC:
      printf("PCAP_SWAPPED_MODIFIED_MAGIC\n");
      break;
  }
  printf("Version major number = %d\n", header->version_major);
  printf("Version minor number = %d\n", header->version_minor);
  printf("GMT to local correction = %d\n", header->thiszone);
  printf("Timestamp accuracy = %d\n", header->sigfigs);
  printf("Snaplen = %d\n", header->snaplen);
  printf("Linktype = %d\n", header->linktype);
}


void my_pkthdr_print(my_pkthdr *pkthdr, int n) {
  static int first_time = 1;
  static int b_sec;
  static int b_usec;
  int c_sec;
  int c_usec;

  if(first_time) {
    first_time = 0;
    b_sec = pkthdr->ts.tv_sec;
    b_usec = pkthdr->ts.tv_usec;
  }

  c_sec = (unsigned) pkthdr->ts.tv_sec - b_sec;
  c_usec = (unsigned) pkthdr->ts.tv_usec - b_usec;
  while(c_usec < 0) {
    c_usec += 1000000;
    c_sec--;
  }

  printf("Packet %d\n", n);
  printf("%05u.%06u\n", (unsigned) c_sec, (unsigned) c_usec);
  printf("Captured Packet Length = %d\n", pkthdr->caplen);
  printf("Actual Packet Length = %d\n", pkthdr->len);
}

void eth_hdr_print(eth_hdr *hdr) {
  printf("Ethernet Header\n");
  char *type;
  switch(ntohs(hdr->eth_type)) {
    case ETH_TYPE_PUP:
      type = "PUP";
      break;
    case ETH_TYPE_IP:
      type = "IP";
      break;
    case ETH_TYPE_ARP:
      type = "ARP";
      break;
    case ETH_TYPE_REVARP:
      type = "REVARP";
      break;
    case ETH_TYPE_8021Q:
      type = "8021Q";
      break;
    case ETH_TYPE_IPV6:
      type = "IPV6";
      break;
    case ETH_TYPE_MPLS:
      type = "MPLS";
      break;
    case ETH_TYPE_MPLS_MCAST:
      type = "MCAST";
      break;
    case ETH_TYPE_PPPOEDISC:
      type = "PPPOEDISC";
      break;
    case ETH_TYPE_PPPOE:
      type = "PPPOE";
      break;
    case ETH_TYPE_LOOPBACK:
      type = "LOOPBACK";
      break;
    default:
      type = UNREC;
      break;
  }
  printf("   %s\n", type);
}

void arp_hdr_print(arp_hdr *hdr) {
  char *operation;
  switch(ntohs(hdr->ar_op)) {
    case ARP_OP_REQUEST:
      operation = "Arp Request";
      break;
    case ARP_OP_REPLY:
      operation = "Arp Reply";
      break;
    case ARP_OP_REVREQUEST:
      operation = "Arp Revrequest";
      break;
    case ARP_OP_REVREPLY:
      operation = "Arp Revreply";
      break;
    default:
      operation = UNREC;
      break;
  }
  printf("      arp operation = %s\n", operation);
}

void ip_hdr_print(ip_hdr *hdr) {
  char *protocol;
  switch(hdr->ip_p) {
    case IP_PROTO_ICMP:
      protocol = "ICMP";
      break;
    case IP_PROTO_IGMP:
      protocol = "IGMP";
      break;
    case IP_PROTO_TCP:
      protocol = "TCP";
      break;
    case IP_PROTO_UDP:
      protocol = "UDP";
      break;
    default:
      protocol = UNREC;
      break;
  }
  printf("      %s\n", protocol);
}


void print_usage(char *filename) {
  printf("Usage: %s file\n", filename);
}

int read_struct(int fdin, void *strct, size_t length) {
  int bytes;
  if((bytes = read(fdin, strct, length)) < 0) {
    perror("read error");
    close(fdin);
    exit(-1);
  }
}
