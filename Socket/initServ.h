#include <stdlib.h>
#include <Winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma once

#define MAX_CONNECTIONS		6
#define	LOOPBACK_ADDRESS	"127.0.0.1"
#define MAX_NAMELEN			25

typedef struct serverInfo{
	SOCKET listener_socket;
	sockaddr_in addr;
	fd_set fr;	// Used for reading reading for new connections and sent data
	fd_set fw;	// Writing
	fd_set fe;	// Exit?
	int maxFd;
	char IP[INET_ADDRSTRLEN];
}serverInfo_t;

typedef struct clientInfo{
	SOCKET clientSocket[MAX_CONNECTIONS];
	sockaddr_in addr;
	char clientIP[MAX_CONNECTIONS][INET_ADDRSTRLEN];
	char clientName[MAX_CONNECTIONS][MAX_NAMELEN];
}clientInfo_t;


int initSocketLib();
SOCKET createSocket();
sockaddr_in initServer(unsigned int port, const char* address);
int bindSocket(serverInfo_t server);
int listenConnection(SOCKET sck, int maxNumOfConnections);
SOCKET acceptConnection(SOCKET sck);