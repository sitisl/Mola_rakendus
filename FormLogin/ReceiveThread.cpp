#include "ReceiveThread.h"

ReceiveThread::ReceiveThread(clientInfo_t clients, QObject* parent)
    : QThread(parent),
    client(clients)
{}

ReceiveThread::~ReceiveThread()
{
}

void ReceiveThread::run() {
    while (true) {
        char buffer[1024] = { 0 };
        int bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
        if (bytesReceived == SOCKET_ERROR) {
            int errorCode = WSAGetLastError();
            qDebug() << "Socket error: " << errorCode;
            break;
        }
        int img_size = 0; // Track the size of the image data received so far
        std::vector<char> img_data; // Use std::vector to hold the image data

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            if (strstr(buffer, "Aktiivsed kasutajad:")) {
                emit usersReceived(QString::fromUtf8(buffer));
            }
            else if (strstr(buffer, "<img")) {
                while (!strstr(buffer, ">"))
                {
                    // Resize the vector to hold the new data
                    img_data.resize(img_size + bytesReceived + 1);

                    // Append the newly received data to the img_data vector
                    std::copy(buffer, buffer + bytesReceived, img_data.begin() + img_size);
                    img_size += bytesReceived;

                    // Read more data from the socket
                    bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
                    if (bytesReceived == SOCKET_ERROR) {
                        int errorCode = WSAGetLastError();
                        qDebug() << "Socket error: " << errorCode;
                        break;
                    }
                }

                // Append the final portion of the image data to the img_data vector

                img_data.resize(img_size + bytesReceived + 1);
                std::copy(buffer, buffer + bytesReceived, img_data.begin() + img_size);
                img_size += bytesReceived;

                emit imageReceived(QByteArray(img_data.data(), img_size));
            }
            else {
                // Display the text message in the text edit
                QString msg = QString::fromUtf8(buffer, bytesReceived);
                QStringList parts = msg.split("<PATH>");
                if (parts.size() == 2) {
                    QString iconPath = parts[0];
                    QString message = parts[1];
                    QString timeStamp = QTime::currentTime().toString("hh:mm:ss");
                    QImage imageAvatar(iconPath);
                    imageAvatar = imageAvatar.scaledToHeight(30, Qt::SmoothTransformation);
                    emit messageReceived(timeStamp + "  " + message, imageAvatar);
                }
                else {
                    emit messageReceived(msg, QImage());
                }
            }
        }
        img_data.clear();
    }
}