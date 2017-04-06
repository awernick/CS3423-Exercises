#ifndef prtlog_h_

#include <sys/types.h>
#include "pcap.h"

#define BUFF_SIZE                       4096

#define PCAP_MAGIC                      0xa1b2c3d4
#define PCAP_SWAPPED_MAGIC              0xd4c3b2a1
#define PCAP_MODIFIED_MAGIC             0xa1b2cd34
#define PCAP_SWAPPED_MODIFIED_MAGIC     0x34cdb2a1

typedef struct pcap_file_header pcap_file_header;
typedef struct eth_hdr eth_hdr;
typedef struct arp_hdr arp_hdr;
typedef struct ip_hdr ip_hdr;

static char UNREC[] = "UNRECOGNIZED";

struct timev {
  unsigned int tv_sec;
  unsigned int tv_usec;
};

typedef struct my_pkthdr {
  struct timev ts;
  int caplen;
  int len;
} my_pkthdr;

void print_usage(char *filename);
int read_struct(int fd, void *strct, size_t length);

// pcap_file_header
void pcap_file_header_print(pcap_file_header *header);

// my_pkthdr
void my_pkthdr_print(my_pkthdr *pkthdr, int n);

// eth_hdr
void eth_hdr_print(eth_hdr *hdr);
void arp_hdr_print(arp_hdr *hdr);
void ip_hdr_print(ip_hdr *hdr);

#endif
