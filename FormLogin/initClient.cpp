#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include "initClient.h"

int initSocketLib()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 0;
    }
    printf("Init success\n");
    return 1;
}

// Returns created socket
SOCKET createSocket() {
    SOCKET socket_return = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_return == INVALID_SOCKET) {
        printf("Socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return NULL;
    }
    printf("Socket created\n");
    return socket_return;
}

void initClient(unsigned int port, sockaddr_in* clientService) {
    InetPton(AF_INET, _T("172.20.10.10"), &clientService->sin_addr.s_addr);
    clientService->sin_family = AF_INET;
    clientService->sin_port = htons(PORT);
    memset(clientService->sin_zero, 0, 8);
}