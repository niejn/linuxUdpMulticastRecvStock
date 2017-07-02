#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>


#include <arpa/inet.h>
#include <unistd.h>
#include "./include/FtdcUserApiStruct.h"
#include "./include/FtdcUserApiDataType.h"

using namespace std;
#define MAXBUFSIZE 65536 // Max UDP Packet size is 64 Kbyte

int main() {
    std::cout << "Hello, World!" << std::endl;





    int sock, status, socklen;

    char buffer[MAXBUFSIZE];
    struct sockaddr_in saddr;
    struct ip_mreq imreq;
    // set content of struct saddr and imreq to zero
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    memset(&imreq, 0, sizeof(struct ip_mreq));

    // open a UDP socket
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if ( sock < 0 )
        perror("Error creating socket"), exit(0);
    saddr.sin_family = PF_INET;
    saddr.sin_port = htons(4096); // listen on port 4096
    saddr.sin_addr.s_addr = htonl(INADDR_ANY); // bind socket to any interface
    status = bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if ( status < 0 )
        perror("Error binding socket to interface"), exit(0);
    //imreq.imr_multiaddr.s_addr = inet_addr("226.0.0.1");
//    imreq.imr_multiaddr.s_addr = inet_addr("234.2.2.2");
    imreq.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    imreq.imr_interface.s_addr = INADDR_ANY; // use DEFAULT interface
// JOIN multicast group on default interface
    status = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                        (const void *)&imreq, sizeof(struct ip_mreq));
    socklen = sizeof(struct sockaddr_in);
    CShfeFtdcMBLMarketDataField *myTicker=(CShfeFtdcMBLMarketDataField *)malloc(sizeof(CShfeFtdcMBLMarketDataField));
    int needRecv=sizeof(CShfeFtdcMBLMarketDataField);
// receive packet from socket
    while(true){

        status = recvfrom(sock, buffer, MAXBUFSIZE, 0,
                          (struct sockaddr *)&saddr, (socklen_t *)&socklen);
        printf("recvfrom Status = %d\n", status);
        printf("\nrecvfrom buffer [%d]bytes\n", sizeof(buffer));
        printf("\nmsg size is [%d]bytes\n", needRecv);

        memcpy(myTicker,buffer,needRecv);
        printf("Direction=%c InstrumentID=%s LastPrice=%f Volume=%d \n", myTicker->Direction,
               myTicker->InstrumentID, myTicker->Price, myTicker->Volume);

        bzero(buffer, needRecv);
        //memcpy(myTicker,buffer,needRecv);
    }
//    status = recvfrom(sock, buffer, MAXBUFSIZE, 0,
//                      (struct sockaddr *)&saddr, (socklen_t *)&socklen);
// shutdown socket
    shutdown(sock, 2);
// close socket
    close(sock);

    return 0;
}