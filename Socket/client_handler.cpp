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
                getClientIcon(client->clientIcon[i], client->clientSocket[i]);
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
                int img_size = 0; // Track the size of the image data received so far
                char* img_data = nullptr; // Allocate a buffer to hold the image data
                int nRet = recv(client->clientSocket[i], receive_buf, sizeof(receive_buf), 0);
                if (nRet == SOCKET_ERROR)
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
                    // Check if the received data is an image
                    if (strstr(receive_buf, "<img"))
                    {
                        // Read the data from the socket until the end of the image tag is found
                        while (!strstr(receive_buf, ">"))
                        {
                            // Reallocate the buffer to hold the new data
                            char* new_img_data = (char*)realloc(img_data, img_size + nRet + 1);
                            if (new_img_data == nullptr)
                            {
                                printf("Error allocating memory for image data.\n");
                                return;
                            }
                            img_data = new_img_data;

                            // Append the newly received data to the img_data buffer
                            memcpy(img_data + img_size, receive_buf, nRet);
                            *(img_data + img_size + nRet) = '\0'; // add null character
                            img_size += nRet;

                            // Read more data from the socket
                            nRet = recv(client->clientSocket[i], receive_buf, sizeof(receive_buf), 0);
                            if (nRet == SOCKET_ERROR)
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
                        }

                        // Append the final portion of the image data to the img_data buffer

                        char* new_img_data = (char*)realloc(img_data, img_size + nRet + 1);
                        if (new_img_data == nullptr)
                        {
                            printf("Error allocating memory for image data.\n");
                            return;
                        }
                        img_data = new_img_data;

                        memcpy(img_data + img_size, receive_buf, nRet);
                        *(img_data + img_size + nRet) = '\0'; // add null character
                        img_size += nRet;

                        printf("%s: Image\n", client->clientName[i]);
                        //printf("%s: %s\n", client->clientName[i], img_data);

                        // Send the image data to all connected clients except the client that sent the message
                        for (int j = 0; j < MAX_CONNECTIONS; j++)
                        {
                            if (client->clientSocket[j] > 0 && client->clientSocket[j] != client->clientSocket[i])
                            {
                                char msg_buf[1024] = { 0 };
                                sprintf(msg_buf, "%s:", client->clientName[i]);
                                send(client->clientSocket[j], msg_buf, strlen(msg_buf), 0);
                                send(client->clientSocket[j], img_data, img_size, 0);
                            }
                        }

                    }
                    else
                    {
                        printf("%s: %s\n", client->clientName[i], receive_buf);
                        // Send the received message to all connected clients except the client that sent the message
                        for (int j = 0; j < MAX_CONNECTIONS; j++)
                        {
                            if (client->clientSocket[j] > 0 && client->clientSocket[j] != client->clientSocket[i])
                            {
                                char msg_buf[1024] = { 0 };
                                sprintf(msg_buf, "%s  %s: %s", client->clientIcon[i], client->clientName[i], receive_buf);
                                send(client->clientSocket[j], msg_buf, strlen(msg_buf), 0);
                            }
                        }
                    }
                }
                free(img_data);
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

void getClientIcon(char* icon, SOCKET clientSocket)
{
    // Send "Choose a nickname" message to the client
    send(clientSocket, "Choose an icon:", strlen("Choose an icon:"), 0);

    // Receive client's chosen nickname
    int bytesReceived = recv(clientSocket, icon, MAX_ICON, 0);

    // Add null terminator to the end of the received string
    icon[bytesReceived] = '\0';

}
