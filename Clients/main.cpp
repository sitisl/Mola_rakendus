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
#include <process.h>
using namespace std;

#define PORT		5555
#define ADDRESS		"127.0.0.1"

DWORD WINAPI sendThreadFunc(LPVOID lpParam);

int main() {

	SOCKET client_sock;
	WSAData wsaData;
	sockaddr_in clientService;

	// Init lib =====================================

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Error intializing Winsock %d", WSAGetLastError());
		return -1;
	}
	printf("Initialized\n");

	// SETUP socket ================================

	client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (client_sock == INVALID_SOCKET) {
		printf("Could not create socket : %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	printf("Socket created\n");
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_family = AF_INET;
	clientService.sin_port = htons(5555);



	int res = connect(client_sock, (struct sockaddr*)&clientService, sizeof(clientService));
	if (res == SOCKET_ERROR) {
		printf("Connect error %d\n", WSAGetLastError());
		WSACleanup();
		closesocket(client_sock);
		return -1;
	}
	printf("Connected\n");
	char buffer[1024] = { 0, };



	while (1) {
		Sleep(10);
		puts("Send: ");
		fgets(buffer, 1023, stdin);
		send(client_sock, buffer, strlen(buffer), 0);
	}
	return 1;
}