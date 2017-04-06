#ifndef logprt_h_

#include <sys/types.h>

#define PCAP_MAGIC                      0xa1b2c3d4
#define PCAP_SWAPPED_MAGIC              0xd4c3b2a1
#define PCAP_MODIFIED_MAGIC             0xa1b2cd34
#define PCAP_SWAPPED_MODIFIED_MAGIC     0x34cdb2a1

typedef struct pcap_file_header {
  u_int32_t magic;
  u_int16_t version_major;
  u_int16_t version_minor;
  u_int32_t thiszone;
  u_int32_t sigfigs;
  u_int32_t snaplen;
  u_int32_t linktype;
} pcap_file_header;

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

void pcap_file_header_print(pcap_file_header *header);

void my_pkthdr_print(my_pkthdr *pkthdr, int n);

#endif
