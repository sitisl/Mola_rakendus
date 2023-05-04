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
                QString usersStr = QString::fromUtf8(buffer);
                QString header = usersStr.left(usersStr.indexOf(":") + 1);
                QString userListStr = usersStr.mid(usersStr.indexOf(":") + 1);
                userListStr = userListStr.trimmed(); // remove any extra whitespace, including newline character
                QStringList usersList = userListStr.split("\n");
                QString formattedUsersStr = header + "<br>";
                for (int i = 0; i < usersList.size(); i++) {
                    formattedUsersStr += "<b>" + usersList[i] + "</b><br>";
                }
                emit usersReceived(formattedUsersStr.toUtf8());
                //emit usersReceived(QString::fromUtf8(buffer));
            }  
            else if (strstr(buffer, "<img"))
            {
                while (!strstr(buffer, "> "))
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
                    imageAvatar = imageAvatar.scaledToHeight(32, Qt::SmoothTransformation);
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

//void ReceiveThread::run() {
//    while (true) {
//        char buffer[1024] = { 0 };
//        int bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
//        if (bytesReceived == SOCKET_ERROR) {
//            int errorCode = WSAGetLastError();
//            qDebug() << "Socket error: " << errorCode;
//            break;
//        }
//
//        if (bytesReceived > 0) {
//            buffer[bytesReceived] = '\0';
//            if (strstr(buffer, "Aktiivsed kasutajad:")) {
//                // Handle user list message
//                QString usersStr = QString::fromUtf8(buffer);
//                QString header = usersStr.left(usersStr.indexOf(":") + 1);
//                QString userListStr = usersStr.mid(usersStr.indexOf(":") + 1);
//                userListStr = userListStr.trimmed(); // remove any extra whitespace, including newline character
//                QStringList usersList = userListStr.split("\n");
//                QString formattedUsersStr = header + "<br>";
//                for (int i = 0; i < usersList.size(); i++) {
//                    formattedUsersStr += "<b>" + usersList[i] + "</b><br>";
//                }
//                emit usersReceived(formattedUsersStr.toUtf8());
//            }
//            else if (strstr(buffer, "<img")) {
//                // Handle image message
//                std::string imgTag(buffer);
//                size_t pos = imgTag.find("size=");
//                if (pos == std::string::npos) {
//                    qDebug() << "Invalid image tag received";
//                    continue;
//                }
//                pos += 6;
//                size_t endPos = imgTag.find('"', pos);
//                if (endPos == std::string::npos) {
//                    qDebug() << "Invalid image tag received";
//                    continue;
//                }
//                int expectedSize = std::stoi(imgTag.substr(pos, endPos - pos));
//
//                std::vector<char> imgData;
//                imgData.reserve(expectedSize);
//
//                int receivedSize = bytesReceived - (endPos + 1);
//                std::copy(buffer + endPos + 1, buffer + bytesReceived, std::back_inserter(imgData));
//
//                while (receivedSize < expectedSize) {
//                    bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
//                    if (bytesReceived == SOCKET_ERROR) {
//                        int errorCode = WSAGetLastError();
//                        qDebug() << "Socket error: " << errorCode;
//                        break;
//                    }
//
//                    if (bytesReceived == 0) {
//                        qDebug() << "Connection closed unexpectedly";
//                        break;
//                    }
//
//                    receivedSize += bytesReceived;
//                    std::copy(buffer, buffer + bytesReceived, std::back_inserter(imgData));
//                }
//
//                if (receivedSize == expectedSize) {
//                    emit imageReceived(QByteArray(imgData.data(), expectedSize));
//                }
//                else {
//                    qDebug() << "Error receiving image data: expected size " << expectedSize
//                        << ", but received " << receivedSize << " bytes";
//                }
//                imgData.clear();
//            }
//            else {
//                // Handle text message
//                QString msg = QString::fromUtf8(buffer, bytesReceived);
//                QStringList parts = msg.split("<PATH>");
//                if (parts.size() == 2) {
//                    QString iconPath = parts[0];
//                    QString message = parts[1];
//                    QString timeStamp = QTime::currentTime().toString("hh:mm:ss");
//                    QImage imageAvatar(iconPath);
//                    imageAvatar = imageAvatar.scaledToHeight(32, Qt::SmoothTransformation);
//                    emit messageReceived(timeStamp + " " + message, imageAvatar);
//                }
//                else {
//                    emit messageReceived(msg, QImage());
//                }
//                
//            }
//        }
//    }
//}
