#include "FormMessenger.h"

FormMessenger::FormMessenger(QString userName, QWidget* parent)
	: QMainWindow(parent),
	m_userName(userName)
{
	ui.setupUi(this);
	setWindowIcon(QIcon(":/myresources/icons/Mola.png"));

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
		//ui.textEdit->append(QString(buffer));
		//ui.textEdit->append(m_userName);

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
	QString imagePath = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
	if (imagePath.isEmpty()) {
		return;
	}

	QImage image(imagePath);
	if (image.isNull()) {
		ui.textEdit->append("Error: Failed to load image");
		return;
	}

	// Scale image to fit in textEdit
	QSizeF size = ui.textEdit->size();
	if (image.width() > size.width() || image.height() > size.height()) {
		image = image.scaled(size.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	// Convert image to Base64 string
	QByteArray imageData;
	QBuffer buffer(&imageData);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, "PNG"); // Use PNG format for better quality
	QString imageString = imageData.toBase64();
	QByteArray imageTag = QString("<img src=\"data:image/png;base64,%1\">").arg(imageString).toUtf8();

	// Send image data over the socket
	send(client.clientSocket, imageTag.data(), imageTag.size(), 0);

	// Append the image tag to the text edit
	ui.textEdit->append(m_userName + ": ");
	ui.textEdit->append(imageTag);
	//ui.textEdit->insertPlainText(imageTag);
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
		int img_size = 0; // Track the size of the image data received so far
		char* img_data = nullptr; // Allocate a buffer to hold the image data

        if (bytesReceived > 0) {
			if (strstr(buffer, "<img")) {
				while (!strstr(buffer, ">"))
				{
					// Reallocate the buffer to hold the new data
					img_data = (char*)realloc(img_data, img_size + bytesReceived + 1);
					if (img_data == nullptr)
					{
						printf("Error allocating memory for image data.\n");
						return;
					}

					// Append the newly received data to the img_data buffer
					memcpy(img_data + img_size, buffer, bytesReceived);
					*(img_data + img_size + bytesReceived) = '\0'; // add null character
					img_size += bytesReceived;

					// Read more data from the socket
					bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
					if (bytesReceived == SOCKET_ERROR) {
						int errorCode = WSAGetLastError();
						qDebug() << "Socket error: " << errorCode;
						break;
					}
				}

				// Append the final portion of the image data to the img_data buffer

				img_data = (char*)realloc(img_data, img_size + bytesReceived + 1);
				if (img_data == nullptr)
				{
					printf("Error allocating memory for image data.\n");
					return;
				}

				memcpy(img_data + img_size, buffer, bytesReceived);
				*(img_data + img_size + bytesReceived) = '\0'; // add null character
				img_size += bytesReceived;
			
				ui.textEdit->append(img_data);
            } 
			else {
                // Display the text message in the text edit
                ui.textEdit->append(buffer);
            }
        }
    }
}




