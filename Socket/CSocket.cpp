#include "CSocket.h"
#include <iostream> // for std::cerr
#include <cstring> // for std::memset


CSocket::CSocket() : m_socket(INVALID_SOCKET), m_acceptedSocket(INVALID_SOCKET)
{
}

CSocket::~CSocket() {
    Close();
}

bool CSocket::Init() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << '\n';
        return false;
    }
#endif
    std::cout << "Init success\n";
    return true;
}

bool CSocket::Bind(int port) {
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << '\n';
        return false;
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(m_socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError() << '\n';
        Close();
        return false;
    }
    std::cout << "Bind success, listening on port:" << port << '\n';
    return true;
}

bool CSocket::Listen(int numConnections) {
    if (listen(m_socket, numConnections) == SOCKET_ERROR) {
        std::cerr << "listen failed with error: " << WSAGetLastError() << '\n';
        Close();
        return false;
    }
    std::cout << "Listen success\n";
    return true;
}

bool CSocket::Accept(CSocket& clientSocket, sockaddr_in& clientAddr) {
    int addrLen = sizeof(clientAddr);
    m_acceptedSocket = accept(m_socket, (sockaddr*)&clientAddr, &addrLen);
    if (m_acceptedSocket == INVALID_SOCKET)
    {
        std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
        return false;
    }
    clientSocket.m_socket = m_acceptedSocket;
    return true;
}

bool CSocket::Send(const char* buffer, int length) {
    int bytesSent = send(m_socket, buffer, length, 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool CSocket::Receive(char* buffer, int bufferSize, int& bytesReceived)
{
    bytesReceived = recv(m_acceptedSocket, buffer, bufferSize, 0);
    if (bytesReceived < 0)
    {
        std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

void CSocket::Close() {
    if (m_socket != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        m_socket = INVALID_SOCKET;
    }
}


