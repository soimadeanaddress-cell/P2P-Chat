#include "../include/tcpSocket.h"
#include <stdio.h>
#include <string.h>

//gcc -o test src/tcpSocket.c -lWs2_32 -lfwpuclnt -lntdsapi
//gcc -o test2 src/tcpSocket.c -lWs2_32 -lfwpuclnt -lntdsapi

char recvbuf[DEFAULT_BUFLEN];
char* portForward = NULL;
int HostRunning = 1;

BOOL socket_MakeSocket(SOCKET* sockPtr, struct addrinfo** addrInfoPtr, char* addr, BOOL serverTrue){

    if(*addrInfoPtr){
        freeaddrinfo(*addrInfoPtr);
        *addrInfoPtr = NULL;
    }

    struct addrinfo hints = {0};
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_protocol   = IPPROTO_TCP;
    
    int int_Result;

    if(serverTrue){
        hints.ai_flags = AI_PASSIVE;
        int_Result= getaddrinfo(NULL, TCPPORT, &hints, addrInfoPtr);
    }else{
        int_Result= getaddrinfo(addr, TCPPORT, &hints, addrInfoPtr);
    }
    if(int_Result!=0) {puts("Failure to Get Address Info"); goto Error;}

    for (struct addrinfo* ptr = *addrInfoPtr; ptr != NULL; ptr=ptr->ai_next){
        SOCKET tempSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (tempSock == INVALID_SOCKET) continue;

        if(serverTrue){
            BOOL off = FALSE;
            setsockopt(tempSock, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&off, sizeof(off));

            int_Result = bind(tempSock, ptr->ai_addr, (int)ptr->ai_addrlen);
            if(int_Result == SOCKET_ERROR){closesocket(tempSock); continue;};

            int_Result = listen(tempSock, SOMAXCONN);
            if(int_Result == SOCKET_ERROR){closesocket(tempSock); continue;};
        }else{
            int_Result = connect(tempSock, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (int_Result == SOCKET_ERROR){closesocket(tempSock); continue;};
        }

        *sockPtr = tempSock;
        break;
    }
    if(*sockPtr==INVALID_SOCKET) goto Error;

    return TRUE;

    Error:
    puts("ERROR MAKING SOCKET");
    freeaddrinfo(*addrInfoPtr);
    *addrInfoPtr = NULL;
    return FALSE;
}

void AcceptSocket(void* connectSocket){
    if(*(SOCKET*)connectSocket == INVALID_SOCKET) _endthread();
    int int_Result;
    puts("Connection to client established\n");
    do
    {
       int_Result = recv(*(SOCKET*)connectSocket, recvbuf, DEFAULT_BUFLEN, 0);
       if (int_Result == 0) goto Cleanup;
       if (int_Result>0){
            if(portForward) send(*(SOCKET*)connectSocket, "E", 2, 0);
            else{
                printf("%s\n", recvbuf);
            }
       }
    } while (int_Result>0 && HostRunning);
    
    Cleanup:
    closesocket(*(SOCKET*)connectSocket);
    _endthread;
}

BOOL socket_connectTo(char* connectAddress){
    SOCKET connectSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    int int_Result;
    int returnVal = 0;
    if(MakeSocket(&connectSocket, &result, connectAddress, FALSE)==0){returnVal = 1; goto Cleanup;}

    puts("Setup complete!");

    send(connectSocket, "FORTNITE\0", 9, 0);

    do{
        int_Result = recv(connectSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (int_Result == 0) goto Cleanup;
        if (int_Result>0){
            if(portForward) send(connectSocket, "", 1, 0);
            else{
                printf("%s\n", recvbuf);
            }
       }
    }while(int_Result>0);

    Cleanup:
    puts("Connection - Cleaning Up\n");
    if(connectSocket!=INVALID_SOCKET) closesocket(connectSocket);
    freeaddrinfo(result);
    return returnVal;
}

int socket_main(){
    SOCKET mainSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
 
    int int_Result;

    if(MakeSocket(&mainSocket, &result, NULL, TRUE)==0)goto Cleanup;

    while(HostRunning){
        puts("Listening");
        SOCKET connectSocket = accept(mainSocket, NULL, NULL);
        if(connectSocket!=INVALID_SOCKET){
            _beginthread(&AcceptSocket, 0, &connectSocket);
        }
    }

    Cleanup:
    puts("Listener - Cleaning Up");
    if(mainSocket!=INVALID_SOCKET) closesocket(mainSocket);
    freeaddrinfo(result);
}

int main(){
    char* E = "127.0.0.1";
    //socket_connectTo(E);
    socket_init();
    return 0;
}