#include "FormMessenger.h"

FormMessenger::FormMessenger(QString userName, QWidget* parent)
	: QMainWindow(parent),
	m_userName(userName)
{
	ui.setupUi(this);
	ui.textEdit->setReadOnly(TRUE);

	// Init lib =====================================

	initSocketLib();

	// SETUP socket =================================

	client.clientSocket = createSocket();

	// Init client with port ========================

	initClient(PORT, &client.addr);

	int res = ::connect(client.clientSocket, (struct sockaddr*)&client.addr, sizeof(client.addr));
	QString connectMsg;
	if (res == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
		connectMsg = QString("Connect error %1\n").arg(errorCode);
		ui.textEdit->append(connectMsg);
		WSACleanup();
		closesocket(client.clientSocket);

	}
	else {
		connectMsg = QString("Connected");
		ui.textEdit->append(connectMsg);
	}
	char buffer[1024] = { 0, };
	int bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
		ui.textEdit->append(QString(buffer));
		ui.textEdit->append(m_userName);

	}
	send(client.clientSocket, m_userName.toUtf8().constData(), m_userName.size(), 0);

	std::thread receiveThread(&FormMessenger::receiveMessages, this);
	receiveThread.detach();
}

FormMessenger::~FormMessenger()
{
	
}


void FormMessenger::on_btnPicture_clicked()
{

}

void FormMessenger::on_btnSend_clicked()
{
	QString message = ui.lineEditMessage->text();
	send(client.clientSocket, message.toUtf8().constData(), message.size(), 0);
	ui.lineEditMessage->clear();
	ui.textEdit->append(m_userName + ": " + message);
}

void FormMessenger::on_btnEmoji_clicked()
{

}

void FormMessenger::receiveMessages()
{
    while (true) {
        char buffer[1024] = { 0 };
        int bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
        if (bytesReceived == SOCKET_ERROR) {
            int errorCode = WSAGetLastError();
            qDebug() << "Socket error: " << errorCode;
            break;
        }
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            ui.textEdit->append(QString(buffer));
        }
    }
}






