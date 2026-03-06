#include "../include/tcpSocket.h"


//gcc -o test src/tcpSocket.c -lWs2_32 -lfwpuclnt
//gcc -o test2 src/tcpSocket.c -lWs2_32 -lfwpuclnt

char recvbuf[DEFAULT_BUFLEN];
int HostRunning = 1;

static BOOL send_all(SOCKET sendingSocket, char* buffer){

    size_t result;
    size_t buffLen = strlen(buffer);
    size_t networkEndian = (size_t)htonll(buffLen);
    char buffLenStr[1+sizeof(networkEndian)];
    buffLenStr[0] = (char)htons(tag_SIZE);
    memcpy(buffLenStr+1, &networkEndian, sizeof(networkEndian));

    //send buffer length:
    result = send(sendingSocket, buffLenStr, sizeof(buffLenStr), 0);
    if(result == SOCKET_ERROR){puts("Failed to send packet size"); return FALSE;}

    //repeat sending until buffer is confirmed to have been sent
    result = 0;
    size_t totalsent = 0;
    while (totalsent>buffLen){
        result = send(sendingSocket, buffer + totalsent, buffLen - totalsent, 0);
        if(result == SOCKET_ERROR){puts("Failed to send packet"); return FALSE;}
        totalsent+=result;
    }
    return TRUE;
}

static char* receive(SOCKET receivingSocket){

    size_t result;
    size_t totalReceived;
    char sizeBuffer[PACKETSIZE_BUFLEN];

    //receive buffer length
    result = recv(receivingSocket, sizeBuffer, PACKETSIZE_BUFLEN, 0);
    if(result == SOCKET_ERROR){puts("Failed to receive packet size"); return NULL;}
    result = ntohs((u_short)sizeBuffer[0]);
    if(result!=tag_SIZE){puts("INVALID TAG - Not size tag"); return NULL;}
    size_t bufferSize;
    memcpy(&bufferSize, bufferSize+1, sizeof(size_t));

    //temporary malloc
    char* ReceivedBuffer = (char*)malloc(bufferSize);

    while(totalReceived < bufferSize){
        result = recv(receivingSocket, ReceivedBuffer+totalReceived, bufferSize-totalReceived, 0);
        if(result == SOCKET_ERROR){puts("Failed to receive packet"); return NULL;}
        totalReceived+=result;
    }

    return ReceivedBuffer;
}

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
    char* errMsg;

    if(serverTrue){
        hints.ai_flags = AI_PASSIVE;
        int_Result= getaddrinfo(NULL, TCPPORT, &hints, addrInfoPtr);
    }else{
        int_Result= getaddrinfo(addr, TCPPORT, &hints, addrInfoPtr);
    }
    if(int_Result!=0) {errMsg ="Failed to get Address Info"; goto Error;}

    for (struct addrinfo* ptr = *addrInfoPtr; ptr != NULL; ptr=ptr->ai_next){
        SOCKET tempSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (tempSock == INVALID_SOCKET) errMsg="Failed to create socket"; continue;

        if(serverTrue){
            BOOL off = FALSE;
            setsockopt(tempSock, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&off, sizeof(off)); //enables connection by IPv4 addresses too.

            int_Result = bind(tempSock, ptr->ai_addr, (int)ptr->ai_addrlen);
            if(int_Result == SOCKET_ERROR){closesocket(tempSock); errMsg="[SERVER] Failed to bind socket"; break;}

            int_Result = listen(tempSock, SOMAXCONN);
            if(int_Result == SOCKET_ERROR){closesocket(tempSock); errMsg="[SERVER] Failed to initialize listening"; break;}
        }else{
            int_Result = connect(tempSock, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (int_Result == SOCKET_ERROR){closesocket(tempSock); errMsg="[CLIENT] Failed to connect to address"; continue;}
        }

        *sockPtr = tempSock;
        break;
    }
    if(*sockPtr==INVALID_SOCKET) goto Error;

    return TRUE;

    Error:
    printf("ERROR - %s - %d\n", errMsg, int_Result);
    freeaddrinfo(*addrInfoPtr); 
    *addrInfoPtr = NULL;
    return FALSE;
}

static void AcceptSocket(void* connectSocket){
    if(*(SOCKET*)connectSocket == INVALID_SOCKET) _endthread();
    int int_Result;
    puts("Connection to client established\n");
    do
    {
       int_Result = recv(*(SOCKET*)connectSocket, recvbuf, DEFAULT_BUFLEN, 0);
       if (int_Result == 0) goto Cleanup;
       if (int_Result>0){ 
            printf("%s\n", recvbuf);
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
    if(socket_MakeSocket(&connectSocket, &result, connectAddress, FALSE)==0){returnVal = 1; goto Cleanup;}

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

void socket_main(){
    SOCKET mainSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
 
    int int_Result;

    if(socket_MakeSocket(&mainSocket, &result, NULL, TRUE)==0)goto Cleanup;

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
    return 0;
}