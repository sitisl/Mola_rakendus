#include "FormMessenger.h"

FormMessenger::FormMessenger(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.textEdit->setReadOnly(TRUE);
	QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
	connect(shortcut, SIGNAL(activated()), this, SLOT(on_btnSend_clicked()));
	ui.btnSound->setStyleSheet("QPushButton {background-color: transparent;border-image: url(:/myresources/icons/notif.png); }");
	isSoundOn = true;
}

FormMessenger::~FormMessenger()
{
	WSACleanup();
	closesocket(client.clientSocket);
	if (m_receiveThread) {
		m_receiveThread->quit();
		m_receiveThread->wait();
		delete m_receiveThread;
		m_receiveThread = nullptr;
	}
}

void FormMessenger::handleClientData(int page, QString username, QString avatarPath)
{
	m_userName = username;
	m_avatarPath = avatarPath;
	initSocketLib();

	// SETUP socket =================================
	
	client.clientSocket = createSocket();

	// Init client with port ========================

	initClient(PORT, &client.addr);

	int res = ::connect(client.clientSocket, (struct sockaddr*)&client.addr, sizeof(client.addr));
	QString connectMsg;
	if (res == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
		connectMsg = QString("\u00DChenduse viga %1").arg(errorCode);
		ui.textEdit->append(connectMsg);
		WSACleanup();
		closesocket(client.clientSocket);
	}
	else {
		connectMsg = QString("\u00DChendatud");
		ui.textEdit->append(connectMsg);
	}
	char buffer[1024] = { 0, };
	int bytesReceived = recv(client.clientSocket, buffer, 1023, 0);

	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
	}
	send(client.clientSocket, m_userName.toUtf8().constData(), m_userName.size(), 0);

	bytesReceived = recv(client.clientSocket, buffer, 1023, 0);
	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
	}
	send(client.clientSocket, m_avatarPath.toUtf8().constData(), m_avatarPath.size(), 0);

	m_receiveThread = new ReceiveThread(client, this);
	connect(m_receiveThread, SIGNAL(usersReceived(QString)), this, SLOT(onUsersReceived(QString)));
	connect(m_receiveThread, SIGNAL(messageReceived(QString, QImage)), this, SLOT(onMessageReceived(QString, QImage)));
	connect(m_receiveThread, SIGNAL(imageReceived(QByteArray)), this, SLOT(onImageReceived(QByteArray)));
	m_receiveThread->start();
}

void FormMessenger::on_btnSound_clicked()
{
	if (isSoundOn) {
		ui.btnSound->setStyleSheet("QPushButton {background-color: transparent;border-image: url(:/myresources/icons/notif-off.png); }");
		ui.btnSound->setToolTip("S\u00F5numi m\u00E4rguanne: v\u00E4ljas");
		isSoundOn = false;

	}
	else {
		ui.btnSound->setStyleSheet("QPushButton {background-color: transparent;border-image: url(:/myresources/icons/notif.png); }");
		isSoundOn = true;
		ui.btnSound->setToolTip("S\u00F5numi m\u00E4rguanne: sees");
	}
	ui.btnSound->update();
}
void FormMessenger::on_btnPicture_clicked()
{
	QString imagePath = QFileDialog::getOpenFileName(this, tr("Vali pilt"), "", tr("Pildid (*.png *.jpg *.jpeg *.bmp)"));
	if (imagePath.isEmpty()) {
		return;
	}

	QImage image(imagePath);
	if (image.isNull()) {
		ui.textEdit->append("Viga: Ei saanud pilti laadida");
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
	QByteArray imageTag = QString("<img src=\"data:image/png;base64,%1\"> ").arg(imageString).toUtf8();

	// Send image data over the socket
	send(client.clientSocket, imageTag.data(), imageTag.size(), 0);

	// Append the image tag to the text edit
	ui.textEdit->append("\n");
	QString timeStamp = QTime::currentTime().toString("hh:mm:ss");
	QImage imageAvatar(m_avatarPath);
	imageAvatar = imageAvatar.scaledToHeight(32, Qt::SmoothTransformation);
	QByteArray imageBytes;
	QBuffer bfrAva(&imageBytes);
	buffer.open(QIODevice::WriteOnly);
	imageAvatar.save(&bfrAva, "PNG");
	QString base64Image = QString::fromLatin1(imageBytes.toBase64().data());
	//Create table for showing the sender nicely
	QString tableRow = "<tr>"
		"<td style='padding: 0px 5px 0px 0px;'><img src='data:image/png;base64," + base64Image + "'/></td>"
		"<td><b><span style='color:#07e3e2'>" + m_userName + "</b></td>"
		"<td><font color='gray'>" + timeStamp + "</font></td>"
		"</tr>";
	// Insert the table row into a new table in the text edit
	ui.textEdit->insertHtml("<table>" + tableRow + "</table></br>");
	ui.textEdit->append(imageTag);
	ui.textEdit->moveCursor(QTextCursor::End);
}

void FormMessenger::on_btnSend_clicked()
{
	QString message = ui.lineEditMessage->text();
	if (message != "" && message.length() < 1024) {
		ui.textEdit->append("\n");
		QString timeStamp = QTime::currentTime().toString("hh:mm:ss");
		// Convert the QString to a wide character string
		LPCWSTR wideStr = (LPCWSTR)message.utf16();
		int wideStrLen = message.length();

		// Calculate the required size of the multibyte character string
		int mbStrLen = WideCharToMultiByte(CP_UTF8, 0, wideStr, wideStrLen, NULL, 0, NULL, NULL);

		// Allocate memory for the multibyte character string and convert the wide character string to it
		char* mbStr = new char[mbStrLen + 1];
		WideCharToMultiByte(CP_UTF8, 0, wideStr, wideStrLen, mbStr, mbStrLen, NULL, NULL);
		mbStr[mbStrLen] = '\0';

		// Send the message using the multibyte character string
		send(client.clientSocket, mbStr, mbStrLen, 0);

		// Free the memory allocated for the multibyte character string
		delete[] mbStr;
		QImage imageAvatar(m_avatarPath);
		imageAvatar = imageAvatar.scaledToHeight(32, Qt::SmoothTransformation);
		QByteArray imageBytes;
		QBuffer buffer(&imageBytes);
		buffer.open(QIODevice::WriteOnly);
		imageAvatar.save(&buffer, "PNG");
		QString base64Image = QString::fromLatin1(imageBytes.toBase64().data());
		//Create table for showing the message nicely
		QString tableRow = "<tr>"
			"<td style='padding: 0px 5px 0px 0px;'><img src='data:image/png;base64," + base64Image + "'/></td>"
			//"<td>" + "<br>" + message + "</td>"
			"<td><b><span style='color:#07e3e2'>" + m_userName + "</b><br>" + message + "< / td>"
			"<td><font color='gray'>" + timeStamp + "</font></td>"
			"</tr>";
		// Insert the table row into a new table in the text edit
		ui.textEdit->insertHtml("<table>" + tableRow + "</table></br>");
		// Clear the message input field
		ui.lineEditMessage->clear();
		// Move the cursor to the end of the text edit
		ui.textEdit->moveCursor(QTextCursor::End);
	}
}

void FormMessenger::on_btnEmoji_clicked()
{
	QDialog* emojiDialog = new QDialog(this);
	QVBoxLayout* emojiLayout = new QVBoxLayout(emojiDialog);
	emojiDialog->setLayout(emojiLayout);
	emojiDialog->setWindowTitle("Emotikonid");

	QScrollArea* scrollArea = new QScrollArea(emojiDialog);
	scrollArea->setWidgetResizable(true);
	emojiLayout->addWidget(scrollArea);

	QWidget* emojiWidget = new QWidget(scrollArea);
	scrollArea->setWidget(emojiWidget);

	QGridLayout* emojiGrid = new QGridLayout(emojiWidget);
	emojiWidget->setLayout(emojiGrid);

	// Add emoji buttons to the grid
	QPushButton* emojiButton = new QPushButton("\U0001F642", emojiWidget);
	emojiGrid->addWidget(emojiButton, 0, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F642"); });

	emojiButton = new QPushButton("\U0001F641", emojiWidget);
	emojiGrid->addWidget(emojiButton, 0, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F641"); });

	emojiButton = new QPushButton("\U0001F603", emojiWidget);
	emojiGrid->addWidget(emojiButton, 1, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F603"); });

	emojiButton = new QPushButton("\U0001F606", emojiWidget);
	emojiGrid->addWidget(emojiButton, 1, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F606"); });

	emojiButton = new QPushButton("\U0001F61B", emojiWidget);
	emojiGrid->addWidget(emojiButton, 2, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F61B"); });

	emojiButton = new QPushButton("\U0001F62E", emojiWidget);
	emojiGrid->addWidget(emojiButton, 2, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F62E"); });

	emojiButton = new QPushButton("\U0001F61C", emojiWidget);
	emojiGrid->addWidget(emojiButton, 3, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F61C"); });

	emojiButton = new QPushButton("\U0001F610", emojiWidget);
	emojiGrid->addWidget(emojiButton, 3, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F610"); });

	emojiButton = new QPushButton("\U0001F62F", emojiWidget);
	emojiGrid->addWidget(emojiButton, 4, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F62F"); });

	emojiButton = new QPushButton("\U0001F616", emojiWidget);
	emojiGrid->addWidget(emojiButton, 4, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F616"); });

	emojiButton = new QPushButton("\U0001F617", emojiWidget);
	emojiGrid->addWidget(emojiButton, 5, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F617"); });

	emojiButton = new QPushButton("\U0001F60F", emojiWidget);
	emojiGrid->addWidget(emojiButton, 5, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F60F"); });

	emojiButton = new QPushButton("\U0001F609", emojiWidget);
	emojiGrid->addWidget(emojiButton, 6, 0);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F609"); });

	emojiButton = new QPushButton("\U0001F44D", emojiWidget);
	emojiGrid->addWidget(emojiButton, 6, 1);
	connect(emojiButton, &QPushButton::clicked, this, [this]() { insertEmoji("\U0001F44D"); });

	QPushButton* closeButton = new QPushButton(tr("Sulge"), emojiDialog);
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
		message.replace(":)", "\U0001F642");
	}
	if (message.contains(":(")) {
		message.replace(":(", "\U0001F641");
	}
	if (message.contains(":D")) {
		message.replace(":D", "\U0001F603");
	}
	if (message.contains("XD")) {
		message.replace("XD", "\U0001F606");
	}
	if (message.contains(":p")) {
		message.replace(":p", "\U0001F61B");
	}
	if (message.contains(":o")) {
		message.replace(":o", "\U0001F62E");
	}
	if (message.contains(";p")) {
		message.replace(";p", "\U0001F61C");
	}
	if (message.contains(":|")) {
		message.replace(":|", "\U0001F610");
	}
	if (message.contains(":O")) {
		message.replace(":O", "\U0001F62F");
	}
	if (message.contains(":S")) {
		message.replace(":S", "\U0001F616");
	}
	if (message.contains(":*")) {
		message.replace(":*", "\U0001F617");
	}
	if (message.contains(":^)")) {
		message.replace(":^)", "\U0001F60F");
	}
	if (message.contains(";)")) {
		message.replace(";)", "\U0001F609");
	}
	ui.lineEditMessage->setText(message);
}

void FormMessenger::onMessageReceived(QString message, QImage avatar)
{
	ui.textEdit->append("\n");
	if (avatar.isNull()) {
		ui.textEdit->append(message);
	}
	else {
		// Extract the timestamp from the input string
		QString timeStamp = message.left(8); // Assuming the timestamp is in the first 8 characters of the input string

		// Split the input string into username and message
		QStringList parts = message.mid(9).split(": "); // Assuming there's a space after the colon
		QString userName = parts[0];
		QString message = parts[1];

		// Convert the avatar image to base64
		QByteArray imageBytes;
		QBuffer buffer(&imageBytes);
		buffer.open(QIODevice::WriteOnly);
		avatar.save(&buffer, "PNG");
		QString base64Image = QString::fromLatin1(imageBytes.toBase64().data());

		// Create the HTML for the new row
		QString newRow = "<tr>"
			"<td style='padding: 0px 5px 0px 0px;'><img src='data:image/png;base64," + base64Image + "'/></td>"
			"<td><b>" + userName + "</b><br>" + message + "</td>"
			"<td><font color='gray'>" + timeStamp + "</font></td>"
			"</tr>";

		// Append the new row to the existing table
		QTextCursor cursor(ui.textEdit->document());
		cursor.movePosition(QTextCursor::End);
		cursor.insertHtml(newRow);

		if (isSoundOn) 
		{
			QMediaPlayer* player = new QMediaPlayer();
			QAudioOutput* audioOutput = new QAudioOutput();
			player->setAudioOutput(audioOutput);
			player->setSource(QUrl("qrc:/sounds/sounds/msg_received.mp3"));
			audioOutput->setVolume(40);
			player->play();
		}

		// Scroll to the bottom of the text edit
		ui.textEdit->moveCursor(QTextCursor::End);

	}
}

void FormMessenger::onUsersReceived(QString users)
{
	QString html = "<html><head></head><body>";
	html += users;
	html += "</body></html>";

	ui.textEditKasutajad->setHtml(html);
	
}

void FormMessenger::onImageReceived(QByteArray image)
{
	ui.textEdit->append(image.data());
	ui.textEdit->moveCursor(QTextCursor::End);

	if (isSoundOn)
	{
		QMediaPlayer* player = new QMediaPlayer();
		QAudioOutput* audioOutput = new QAudioOutput();
		player->setAudioOutput(audioOutput);
		player->setSource(QUrl("qrc:/sounds/sounds/msg_received.mp3"));
		audioOutput->setVolume(40);
		player->play();
	}	
}
