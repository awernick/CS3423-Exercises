#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "logprt.h"

void printTime(struct my_pkthdr p);

int main (int argc, char *argv[]) {
    int fdin;
    int pnum = 0;
    struct pcap_file_header fheader;
    struct timev time;
    struct my_pkthdr pheader;
    char *pbody;
    char buf[4096];
    int firsttime;
    unsigned int b_sec, c_sec;
    int b_usec, c_usec;

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
        printf("Packet %d\n", pnum++);
        printTime(pheader);
        printf("Captured Packet Length = %d\n", pheader.caplen);
        printf("Actual Packet Length = %d\n\n", pheader.len);

        if( read(fdin, buf, pheader.len) != pheader.len)
            exit(-1);

        pbody = (char *)buf;
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
