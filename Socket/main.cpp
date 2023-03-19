#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include "initServ.h"
#include "client_handler.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 5555

const char szHost[] = "www.google.com";



int main(const int argc, const char* argv[]) {

	serverInfo_t server{0,};
	clientInfo_t client{0,};

	// Init WINSOCK
	initSocketLib();

	server.listener_socket = createSocket(); // Create socket 

	server.addr = initServer(PORT, LOOPBACK_ADDRESS); // Init server with port and address, address can be loopback or the IPv4 of server PC

	if (bindSocket(server) != 1) return 0;	// Bind socket and server

	int addrLen = sizeof(server.addr);
	if (getsockname(server.listener_socket, (sockaddr*)&server.addr, &addrLen) == SOCKET_ERROR) {
		printf("Couldnt get server name\n");
		return 0;
	}

	inet_ntop(AF_INET, &(server.addr.sin_addr), server.IP, INET_ADDRSTRLEN);

	// Print the server IP address
	printf("Server IP: %s\n", server.IP);

	if (listenConnection(server.listener_socket, MAX_CONNECTIONS) != 1) return 0; // The entered socket is always listening.

	// TODO: create a multiple client handler
	// There are 2 options to do this,
	// 1) Is to use multithreading with <thread> header
	// 2) RECOMMENDED, is to use fd_set data structure, and select() function
	// 
	// TODO: If multi client done, let each client choose their username
	// The ip aadress is always the same for incoming clients, only the PORT is unique
	// This means we can use assign each port a different name, somehow

	struct timeval tv = { 1,0 }; // 1 sec, 0 us

	// Used in select function, that handles fd_set.

	

	server.maxFd = server.listener_socket + 1;
	int nRet = 0;

	while (1) {

		FD_ZERO(&server.fr);
		FD_SET(server.listener_socket, &server.fr);

		for (int i = 0; i < MAX_CONNECTIONS; i++) // This loops over max client sockets 
		{										  // and sets them to be reading sockets	
			if (client.clientSocket[i] > 0)
			{
				FD_SET(client.clientSocket[i], &server.fr);
			}
		}

		nRet = select(MAX_CONNECTIONS, &server.fr, NULL, NULL, &tv);
		if (nRet < 0) {
			printf("Select API call failed %d\n", WSAGetLastError());
			return 0;
		}
		else if (nRet == 0) {
			//printf("No client at port waiting for connections\n");
		}
		else 
		{ 
			// This is true when client waiting to connect
		    // or data to be received from client

			if (FD_ISSET(server.listener_socket, &server.fr)) // Hanlde new connection
			{
				HandleNewConnection(&server, &client);
			}
			else
			{
				//printf("Client sent some data\n");
				HandleClientData(&server,&client);
			}
		}
	}

	//----------------Handles only one client--------------------

	//SOCKET accepted_sck = acceptConnection(server.socket); // For incoming connections a new socket is created

	//int nRet;
	//char buf[1024];
	//char clientIP[INET_ADDRSTRLEN];

	//getClientIP(clientIP, accepted_sck);
	//printf("Client IP address: %s\n", clientIP);

	//while (1) {
	//	memset(buf, 0, 1024);

	//	nRet = recv(accepted_sck, buf, sizeof(buf), 0);
	//	if (nRet > 0) {
	//		printf("%s sent: %s\n", clientIP, buf);
	//	}
	//	else if (nRet == 0) {
	//		printf("Did not reveive\n");
	//	}
	//	else {
	//		printf("Client disconnected, closing socket\n");
	//		closesocket(server.socket);
	//		WSACleanup();
	//		return 1;
	//	}
	//}
	////--------------------------------------------------------

	return 1;
}

	//-----------------------EXPERIMENTAL-----------------------------------

//	CSocket m_csocket;
//	
//	m_csocket.Init();
//	m_csocket.Bind(PORT);
//	m_csocket.Listen(1);
//
//
//	
//	while (1) {
//		CSocket clientSocket;
//		sockaddr_in clientAddr;
//		if (!m_csocket.Accept(clientSocket, clientAddr)) {
//			cerr << "Failed to accept incoming connection" << endl;
//			return 1;
//		}
//		cout << "Incoming connection from " << inet_ntoa(clientAddr.sin_addr)
//		<< ":" << ntohs(clientAddr.sin_port) << std::endl;
//
//		char buffer[1024];
//		int bytesReceived;
//		do {
//			bytesReceived = clientSocket.Receive(buffer, 1024, bytesReceived);
//			printf("%s\n", buffer);
//			if (bytesReceived > 0) {
//				clientSocket.Send(buffer, bytesReceived);
//			}
//		} while (bytesReceived > 0);
//	}
//
//	SOCKET sock;
//	sockaddr_in server;
//
//	return 1;
//
//	// Create Socket (close socket afterwards closesocket() )
//
//	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//
//	if (sock == SOCKET_ERROR) {
//		printf("Could not create socket : %d\n", WSAGetLastError());
//		WSACleanup();
//		ExitProcess(EXIT_SUCCESS);
//	}
//
//	printf("Socket created\n");
//
//	//Init server info
//
//	server.sin_family = AF_INET;
//	//InetPton(AF_INET, _T("127.0.0.1"), &server.sin_addr.s_addr);
//	server.sin_port = htons(PORT);
//	server.sin_addr.s_addr = inet_addr("192.168.0.24");
//	memset(&server.sin_zero, 0, 8);
//
//	// Bind socket - this assosciates a local address with a socket
//	if (bind(sock, (SOCKADDR *) &server, sizeof(server)) == SOCKET_ERROR) {
//		printf("Could not bind socket %d", WSAGetLastError());
//		closesocket(sock);
//		WSACleanup();
//		return 0;
//	}
//	printf("Bind complete\n");
//
//	// Listen for incoming connections listen(socket, maxNumOfConnections);
//	// Sets sock into listening mode
//
//	if (listen(sock, 1) == SOCKET_ERROR)
//		printf("Listen() error %d", WSAGetLastError());
//	else
//	printf("Listen() ok, waiting for connections\n");
//
//	// Accept connections accept() - blocking function, pauses server until
//	// a client has connected to the listening socket, we're using "sock"
//	// Returns a copy of the listening socket, we're using "accepted_sock"
//	// accepted_sock will handle the communication with client and server, while
//	// sock will keep on listening and accepting new clients
//	
//	SOCKET accepted_sock;
//	accepted_sock = accept(sock, NULL, NULL);
//	if (accepted_sock == INVALID_SOCKET) {
//		printf("Accept failed %d", WSAGetLastError());
//		return -1;
//	}
//
//	fd_set readSet{};
//	FD_ZERO(&readSet);
//	FD_SET(accepted_sock, &readSet);
//
//	timeval timeout = { 15, 0 };
//
//	int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);
//
//
//
//	//char buf[1024] = { 0, };
//	while (1) {
//
//		memset(buf, 0, 1024);
//
//		int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);
//
//		if (selectResult == SOCKET_ERROR) {
//			// handle error
//		}
//		
//		else if (selectResult == 0) {
//			// no data available, continue
//		}
//
//		else {
//			if (FD_ISSET(sock, &readSet)) {
//				// accept the new connection and add the client socket to the readSet
//				SOCKET clientSocket = accept(sock, nullptr, nullptr);
//				FD_SET(clientSocket, &readSet);
//			}
//		}
//
//		for (unsigned int i = 0; i < readSet.fd_count; i++) {
//			SOCKET socket = readSet.fd_array[i];
//			if (socket != sock && FD_ISSET(socket, &readSet)) {
//				char buffer[1024];
//				int recvResult = recv(socket, buffer, sizeof(buffer), 0);
//				if (recvResult == SOCKET_ERROR) {
//					// handle error
//				}
//				else if (recvResult == 0) {
//					printf("Client disconnected");
//					// client has disconnected, remove the socket from the readSet
//					closesocket(socket);
//					FD_CLR(socket, &readSet);
//				}
//				else {
//					// handle the received data from the client
//					nRet = recv(accepted_sock, buf, sizeof(buf), 0);
//					if (nRet < 0) {
//						printf("Couldnt receive message %d\n", WSAGetLastError());
//					}
//					else {
//						printf("Receieved %s\n", buf);
//					}
//				}
//			}
//		}
//	}
//	
//
//	ExitProcess(EXIT_SUCCESS);
//}
