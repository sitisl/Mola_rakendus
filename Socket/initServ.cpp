#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <tchar.h>

#include "initServ.h"

// Inits lib
int initSocketLib() 
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << '\n';
        return 0;
    }
    std::cout << "Init success\n";
    return 1;
}

// Returns created socket
SOCKET createSocket() { 
    SOCKET socket_return = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_return == INVALID_SOCKET) {
        std::cerr << "Socket failed with error: " << WSAGetLastError() << '\n';
        WSACleanup();
        return NULL;
    }
    std::cout << "Socket created\n";
    return socket_return;
    
}

// Inits server socket with specific address and port
sockaddr_in initServer(unsigned int port, const char* address){
    sockaddr_in addr_in;

    //InetPton(AF_INET, _T("127.0.0.1"), &server.sin_addr.s_addr);

    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(port);
    //addr_in.sin_addr.s_addr = inet_addr(address); 
    addr_in.sin_addr.s_addr = INADDR_ANY; //Only for server
    //addr_in.sin_addr.s_addr = inet_addr("192.168.0.24");
    memset(&addr_in.sin_zero, 0, 8);

    return addr_in;
}

static void getServerIP(serverInfo_t server) {
    int addrLen = sizeof(server.addr);
    if (getsockname(server.listener_socket, (sockaddr*)&server.addr, &addrLen) == SOCKET_ERROR) {
        printf("Couldnt get server name\n");
        return;
    }

    inet_ntop(AF_INET, &(server.addr.sin_addr), server.IP, INET_ADDRSTRLEN);

    // Print the server IP address
    printf("Server IP: %s\n", server.IP);
}

// Bind the created socket with the server
int bindSocket(serverInfo_t server) {

    // Bind socket - this assosciates a local IP address with the socket
    if (bind(server.listener_socket, (SOCKADDR*)&server.addr, sizeof(server.addr)) == SOCKET_ERROR) {
        printf("Could not bind socket %d", WSAGetLastError());
        closesocket(server.listener_socket);
        WSACleanup();
        return 0;
    }
    printf("Bind complete\n");
    return 1;
}

// Listen for incoming connection, set max amount of connection
int listenConnection(SOCKET sck, int maxNumOfConnections) {
    if (listen(sck, maxNumOfConnections) == SOCKET_ERROR) {
        printf("Listen() error %d", WSAGetLastError());
        return 0;
    }
    else {
        printf("Listen() ok, waiting for connections\n");
        return 1;
    }
}

// Accept connections accept() - blocking function, pauses server until
// a client has connected to the listening socket, we're using "sock"
// Returns a copy of the listening socket, we're using "accepted_sock"
// accepted_sock will handle the communication with client and server, while
// sock will keep on listening and accepting new clients

SOCKET acceptConnection(SOCKET sck){

    SOCKET accepted_sck;
    accepted_sck = accept(sck, NULL, NULL);
    if (accepted_sck == INVALID_SOCKET) {
        printf("Accept failed");
        return 0;

    }printf("Accept successful\n");
    return accepted_sck;
}



