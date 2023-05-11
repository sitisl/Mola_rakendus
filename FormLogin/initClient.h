#pragma once

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <tchar.h>

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