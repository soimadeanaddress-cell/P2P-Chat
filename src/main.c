#include "../include/tcpSocket.h"
#include "../include/arena_pool.h"
#include <stdio.h>

int main() {
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa)!=0){
        WSACleanup();
        return 1;
    }

    struct arena memArena = {};

    //_beginthread();

    return 0;
}