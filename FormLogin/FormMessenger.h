#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <QMainWindow>
#include "ui_FormMessenger.h"
#include "initClient.h"
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QThread>
#include <QBuffer>
#include <QPixmap>
#include <QLabel>
#include <QShortcut>
#include <QScrollArea>
#include "ReceiveThread.h"


using namespace std;

class FormMessenger : public QMainWindow
{
	Q_OBJECT

public:
	FormMessenger(QWidget *parent = nullptr);
	~FormMessenger();

private:
	Ui::FormMessengerClass ui;
	QString m_userName;
	QString m_avatarPath;
	clientInfo_t client;
	ReceiveThread* m_receiveThread;

public slots:
	void handleClientData(int page, QString username, QString avatarPath);

private slots:
	void on_btnPicture_clicked();
	void on_btnSend_clicked();
	void on_btnEmoji_clicked();
	void insertEmoji(const QString& emoji);
	void on_lineEditMessage_textChanged();
	void onMessageReceived(QString message, QImage avatar);
	void onUsersReceived(QString);
	void onImageReceived(QByteArray);
	//void receiveMessages();

};

