#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include "initClient.h"


int main() {

	clientInfo_t client = { 0, };
	
	// Init lib =====================================

	initSocketLib();

	// SETUP socket =================================

	client.clientSocket = createSocket();

	// Init client with port ========================

	initClient(PORT, &client.addr); 

	int res = connect(client.clientSocket, (struct sockaddr*)&client.addr, sizeof(client.addr));
	if (res == SOCKET_ERROR) {
		printf("Connect error %d\n", WSAGetLastError());
		WSACleanup();
		closesocket(client.clientSocket);
		return -1;
	}
	printf("Connected\n");
	char buffer[1024] = { 0, };
	int bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
		printf("%s\n", buffer);
	}
	fgets(buffer, 1023, stdin);
	size_t len = strlen(buffer);
	if (len > 0 && buffer[len - 1] == '\n') {
		buffer[len - 1] = '\0';
	}
	// Send chosen nickname to server
	send(client.clientSocket, buffer, (int)strlen(buffer), 0);


	while (1) {
		
		Sleep(10);
		puts("Send: ");
		fgets(buffer, 1023, stdin);
		send(client.clientSocket, buffer, (int)strlen(buffer), 0);
	}
	return 1;
}