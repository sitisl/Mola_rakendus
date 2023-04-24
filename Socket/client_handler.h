#pragma once
#include "initServ.h"

void HandleNewConnection(serverInfo_t* serv, clientInfo_t* client);
void HandleClientData(serverInfo_t* serv, clientInfo_t* client);
void getClientIP(char* buf, SOCKET accepted_sck);
void getClientName(char* buf, SOCKET accepted_sck);
void getClientIcon(char* buf, SOCKET accepted_sck);
void sendActiveUsers(clientInfo_t* client);


