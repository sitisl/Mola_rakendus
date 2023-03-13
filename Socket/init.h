#include <stdlib.h>
#include <Winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CONNECTIONS 6

typedef struct {
	SOCKET socket;
	SOCKET socketAccepted;
	sockaddr_in addr;
	sockaddr_in addrAccepted[MAX_CONNECTIONS];
}socket_t;

int initSocketLib();
SOCKET createSocket();
sockaddr_in initServer(unsigned int port, const char* address);
int bindSocket(socket_t server);
int listenConnection(SOCKET sck, int maxNumOfConnections);
SOCKET acceptConnection(SOCKET sck);
char* getClientIP(char *buf, SOCKET accepted_sck);
