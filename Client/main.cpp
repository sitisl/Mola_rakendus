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
using namespace std;

#define PORT 55555

int main() {

	SOCKET client_sock;
	WSAData wsaData;
	WORD ver = MAKEWORD(2, 2);
	sockaddr_in clientService;

	// Init lib

	if (WSAStartup(ver, &wsaData) != 0) {
		printf("Error intializing Winsock %d", WSAGetLastError());
		return -1;
	}

	// Create socket

	client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (client_sock == SOCKET_ERROR) {
		printf("Could not create socket : %d\n", WSAGetLastError());
		WSACleanup();
		ExitProcess(EXIT_SUCCESS);
	}

	// Connect to server socket, default wait is 75s
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(PORT);
	if (connect(client_sock, (SOCKADDR*) &clientService.sin_addr.s_addr, sizeof(clientService)) == SOCKET_ERROR) {
		printf("Failed to connect\n");
		WSACleanup();
		return -1;
	}
	else {
		printf("Client connected\nClient able to send and receieve data\n");
	}

	return 1;
}