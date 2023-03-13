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
#include <future>

#include <stdio.h>
#include <stdlib.h>

void on_client_connect(SOCKET client);
using namespace std;

int main()
{
	WSADATA wsa_data;
	SOCKADDR_IN server_addr, client_addr;

	WSAStartup(MAKEWORD(2, 2), &wsa_data);
	const auto server = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5555);

	::bind(server, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr));
	listen(server, 0);

	cout << "Listening for incoming connections..." << endl;

	int client_addr_size = sizeof(client_addr);

	while(1)
	{
		SOCKET client;

		if ((client = accept(server, reinterpret_cast<SOCKADDR*>(&client_addr), &client_addr_size)) != INVALID_SOCKET)
		{
			auto fut = async(launch::async, on_client_connect, client);
		}

		const auto last_error = WSAGetLastError();

		if (last_error > 0)
		{
			cout << "Error: " << last_error << endl;
		}
	}
}

void on_client_connect(SOCKET client)
{
	char buffer[500];

	cout << "Client connected!" << endl;
	recv(client, buffer, sizeof(buffer), 0);
	cout << "Client says: " << buffer << endl;
	memset(buffer, 0, sizeof(buffer));

	closesocket(client);
	cout << "Client disconnected." << endl;
}