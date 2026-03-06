#ifndef socket_h
#define socket_h
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <rpc.h>

#define uint8 unsigned char
#define TCPPORT "6767" //hehe I am very hilarious
#define DEFAULT_BUFLEN 1024*8 //8KiB
#define PACKETSIZE_BUFLEN 9

typedef enum{
    tag_MSGINFO = 0,
    tag_USERINFO = 1,
    tag_SIZE = 2,
    tag_MSG = 3,
}PacketTags;

BOOL socket_MakeSocket(SOCKET* sockPtr, struct addrinfo** addrInfoPtr, char* addr, BOOL serverTrue);

BOOL socket_connectTo(char* connectAddress);

void socket_listen();

#endif