#ifndef socket_h
#define socket_h
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <ntdsapi.h>

#define TCPPORT "6767" //hehe I am very hilarious
#define DEFAULT_BUFLEN 512

BOOL socket_MakeSocket(SOCKET* sockPtr, struct addrinfo** addrInfoPtr, char* addr, BOOL serverTrue);

BOOL socket_connectTo(char* connectAddress);

int socket_listen();

#endif