#include <stdlib.h>
#include <Winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CONNECTIONS 6
#define	LOOPBACK_ADDRESS "127.0.0.1"

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
void getClientIP(char *buf, SOCKET accepted_sck);
