#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

class CSocket {
public:
    CSocket();
    ~CSocket();
    bool Init();
    bool Bind(int port);
    bool Listen(int backlog);
    bool Accept(CSocket& clientSocket, sockaddr_in& clientAddr);
    bool Send(const char* buffer, int length);
    bool Receive(char* buffer, int length, int& bytesReceived);
    void Close();
    SOCKET GetSocket() const { return m_socket; }
    SOCKET GetAcceptedSocket() const { return m_acceptedSocket; }

private:
    SOCKET m_socket;
    SOCKET  m_acceptedSocket;
};

#endif // SOCKET_H
