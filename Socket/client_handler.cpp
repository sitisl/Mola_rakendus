#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include "Client_handler.h"


void HandleNewConnection(serverInfo_t* serv, clientInfo_t* client)
{	
	
	// nNewClient will be a new file descriptor
	// and not the client communicaiton will take place
	// using this socket/file descriptor only
	SOCKET nNewClient = accept(serv->listener_socket, NULL, NULL);

	if (nNewClient == INVALID_SOCKET)
	{
		printf("Unable to get new client socket %d\n", WSAGetLastError());
		return;
	}
	else
	{
		int i;
		for (i = 0; i < MAX_CONNECTIONS; i++)
		{
			if (client->clientSocket[i] == 0) // If no socket assigned 
			{
				client->clientSocket[i] = nNewClient; // Assign new socket
				getClientIP(client->clientIP[i], client->clientSocket[i]);
				getClientName(client->clientName[i], client->clientSocket[i]);
				printf("%s connected IP: %s\n",client->clientName[i], client->clientIP[i]);
				if (nNewClient > serv->maxFd) {
					serv->maxFd = nNewClient + 1;
				}
				break;
			}
		}
		if (i == MAX_CONNECTIONS)
		{
			printf("Server reached max client limit of %d connections\n", MAX_CONNECTIONS);
		}
	}
}

void HandleClientData(serverInfo_t* serv, clientInfo_t* client)
{
    fd_set read_fds = serv->fr;
    int nRet = select(serv->maxFd, &read_fds, NULL, NULL, NULL);
    if (nRet == SOCKET_ERROR)
    {
        printf("Select API call failed in func %d\n", WSAGetLastError());
        return;
    }
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (client->clientSocket[i] > 0) // True only if socket is assigned 
        {
            if (FD_ISSET(client->clientSocket[i], &serv->fr))
            {
                // Read client data
                char receive_buf[1024] = { 0, };
                int nRet = recv(client->clientSocket[i], receive_buf, sizeof(receive_buf), 0);
                if (nRet  == SOCKET_ERROR)
                {
                    printf("%s Disconnected or error, closing socket\n", client->clientName[i]);
                    FD_CLR(client->clientSocket[i], &serv->fr); // Remove socket from fd_set
                    closesocket(client->clientSocket[i]);
                    client->clientSocket[i] = 0; // Set the socket to 0 so it can be reused later
                }
                else if (nRet == 0) {
                    printf("%s Disconnected closing socket\n", client->clientName[i]);
                    FD_CLR(client->clientSocket[i], &serv->fr); // Remove socket from fd_set
                    closesocket(client->clientSocket[i]);
                    client->clientSocket[i] = 0; // Set the socket to 0 so it can be reused later
                }
                else
                {
                    printf("%s: %s\n",client->clientName[i], receive_buf);

                    // Send the received message to all connected clients except the client that sent the message
                    for (int j = 0; j < MAX_CONNECTIONS; j++)
                    {
                        if (client->clientSocket[j] > 0 && client->clientSocket[j] != client->clientSocket[i])
                        {
							char msg_buf[1024] = { 0 };
							sprintf(msg_buf, "%s: %s", client->clientName[i], receive_buf);
							send(client->clientSocket[j], msg_buf, strlen(msg_buf), 0);
                        }
                    }
                }
            }
        }
    }
}

void getClientIP(char* buf, SOCKET accepted_sck) {
	sockaddr_in clientAddr;
	char stemp[INET_ADDRSTRLEN];
	int clientAddrLen = sizeof(clientAddr);
	getpeername(accepted_sck, (SOCKADDR*)&clientAddr, &clientAddrLen);
	inet_ntop(AF_INET, &clientAddr.sin_addr, stemp, INET_ADDRSTRLEN);
	int client_port = ntohs(clientAddr.sin_port);
	snprintf(buf, INET_ADDRSTRLEN, "%s:%d", stemp, client_port);
}

void getClientName(char* name, SOCKET clientSocket)
{

	// Send "Choose a nickname" message to the client
	send(clientSocket, "Choose a nickname:", strlen("Choose a nickname:"), 0);

	// Receive client's chosen nickname
	int bytesReceived = recv(clientSocket, name, MAX_NAMELEN, 0);

	// Add null terminator to the end of the received string
	name[bytesReceived] = '\0';

}
