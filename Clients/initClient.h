#pragma once

#include <stdlib.h>
#include <Winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_NAMELEN				25
#define PORT					5555
#define LOOPBACK_ADDRESS		"127.0.0.1"

typedef struct clientInfo {
	SOCKET clientSocket;
	sockaddr_in addr;
	char clientIP[INET_ADDRSTRLEN];
	char clientName[MAX_NAMELEN];
}clientInfo_t;

int initSocketLib();
SOCKET createSocket();
void initClient(unsigned int port, sockaddr_in *clientService);