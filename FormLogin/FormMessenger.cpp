#include "FormMessenger.h"

FormMessenger::FormMessenger(QString userName, QString avatar, QWidget* parent)
	: QMainWindow(parent),
	m_userName(userName),
	m_avatarPath(avatar)
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
		connectMsg = QString("Connect error %1").arg(errorCode);
		ui.textEdit->append(connectMsg);
		WSACleanup();
		closesocket(client.clientSocket);
		//ui.centralWidget->setEnabled(false);
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

	bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
	}
	send(client.clientSocket, m_avatarPath.toUtf8().constData(), m_avatarPath.size(), 0);

	QThread* thread = new QThread;
	this->moveToThread(thread);

	// Connect the thread's started signal to the receiveMessages function
	connect(thread, &QThread::started, this, &FormMessenger::receiveMessages);

	// Start the thread
	thread->start();

	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
	connect(shortcut, SIGNAL(activated()), this, SLOT(on_btnSend_clicked()));

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
	image.save(&buffer, "PNG");
	QString imageString = imageData.toBase64();
	QByteArray imageTag = QString("<img src=\"data:image/png;base64,%1\">").arg(imageString).toUtf8();

	// Send image data over the socket
	send(client.clientSocket, imageTag.data(), imageTag.size(), 0);

	send(client.clientSocket, m_avatarPath.toUtf8().constData(), m_avatarPath.size(), 0);

	// Append the image tag to the text edit
	ui.textEdit->append("\n");
	QString timeStamp = QTime::currentTime().toString("hh:mm:ss");
	QImage imageAvatar(m_avatarPath);
	imageAvatar = imageAvatar.scaledToHeight(30, Qt::SmoothTransformation);
	QTextCursor cursor(ui.textEdit->textCursor());
	cursor.insertImage(imageAvatar);
	cursor.insertText(timeStamp + "  " + m_userName + ": ");
	ui.textEdit->append(imageTag);
	ui.textEdit->moveCursor(QTextCursor::End);
	//ui.textEdit->insertPlainText(imageTag);
}

void FormMessenger::on_btnSend_clicked()
{
	QString message = ui.lineEditMessage->text();
	if (message != "" && message.length() < 1024) {
		ui.textEdit->append("\n");
		QString timeStamp = QTime::currentTime().toString("hh:mm:ss");
		send(client.clientSocket, message.toUtf8().constData(), message.size(), 0);
		QImage imageAvatar(m_avatarPath);
		imageAvatar = imageAvatar.scaledToHeight(30, Qt::SmoothTransformation);
		QTextCursor cursor(ui.textEdit->textCursor());
		cursor.insertImage(imageAvatar);
		cursor.insertText(timeStamp + "  "  + m_userName + ": " + message);
		ui.lineEditMessage->clear();
		ui.textEdit->moveCursor(QTextCursor::End);
	}
}

void FormMessenger::on_btnEmoji_clicked()
{
	QDialog* emojiDialog = new QDialog(this);
	QVBoxLayout* emojiLayout = new QVBoxLayout(emojiDialog);
	emojiDialog->setLayout(emojiLayout);
	emojiDialog->setWindowTitle("Emojis");

	QScrollArea* scrollArea = new QScrollArea(emojiDialog);
	scrollArea->setWidgetResizable(true);
	emojiLayout->addWidget(scrollArea);

	QWidget* emojiWidget = new QWidget(scrollArea);
	scrollArea->setWidget(emojiWidget);

	QGridLayout* emojiGrid = new QGridLayout(emojiWidget);
	emojiWidget->setLayout(emojiGrid);

	// Add emoji buttons to the grid
	QPushButton* emojiButton = new QPushButton(QString::fromUtf8("\U0001F642"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 0, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F642")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F641"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 0, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F641")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F603"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 1, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F603")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F606"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 1, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F606")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F61B"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 2, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F61B")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F62E"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 2, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F62E")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F61C"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 3, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F61C")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F610"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 3, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F610")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F62F"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 4, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F62F")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F616"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 4, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F616")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F617"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 5, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F617")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F60F"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 5, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F60F")); });

	emojiButton = new QPushButton(QString::fromUtf8("\U0001F609"), emojiWidget);
	emojiGrid->addWidget(emojiButton, 6, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji(QString::fromUtf8("\U0001F609")); });

	QPushButton* closeButton = new QPushButton(tr("Close"), emojiDialog);
	emojiLayout->addWidget(closeButton);
	connect(closeButton, &QPushButton::clicked, emojiDialog, &QDialog::reject);

	emojiDialog->exec();
}

void FormMessenger::insertEmoji(const QString& emoji)
{
	ui.lineEditMessage->insert(emoji);
}

void FormMessenger::on_lineEditMessage_textChanged()
{
	QString message = ui.lineEditMessage->text();
	if (message.contains(":)")) {
		message.replace(":)", QString::fromUtf8("\U0001F642"));
	}
	if (message.contains(":(")) {
		message.replace(":(", QString::fromUtf8("\U0001F641"));
	}
	if (message.contains(":D")) {
		message.replace(":D", QString::fromUtf8("\U0001F603"));
	}
	if (message.contains("XD")) {
		message.replace("XD", QString::fromUtf8("\U0001F606"));
	}
	if (message.contains(":p")) {
		message.replace(":p", QString::fromUtf8("\U0001F61B"));
	}
	if (message.contains(":o")) {
		message.replace(":o", QString::fromUtf8("\U0001F62E"));
	}
	if (message.contains(";p")) {
		message.replace(";p", QString::fromUtf8("\U0001F61C"));
	}
	if (message.contains(":|")) {
		message.replace(":|", QString::fromUtf8("\U0001F610"));
	}
	if (message.contains(":O")) {
		message.replace(":O", QString::fromUtf8("\U0001F62F"));
	}
	if (message.contains(":S")) {
		message.replace(":S", QString::fromUtf8("\U0001F616"));
	}
	if (message.contains(":*")) {
		message.replace(":*", QString::fromUtf8("\U0001F617"));
	}
	if (message.contains(":^)")) {
		message.replace(":^)", QString::fromUtf8("\U0001F60F"));
	}
	if (message.contains(";)")) {
		message.replace(";)", QString::fromUtf8("\U0001F609"));
	}
	ui.lineEditMessage->setText(message);
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
		std::vector<char> img_data; // Use std::vector to hold the image data

		if (bytesReceived > 0) {
			buffer[bytesReceived] = '\0';
			if (strstr(buffer, "<img")) {
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

				ui.textEdit->append(img_data.data());
			}
			else {
				// Display the text message in the text edit
				ui.textEdit->append(QString(buffer));
			}
			ui.textEdit->moveCursor(QTextCursor::End);
		}
		img_data.clear();
	}
}