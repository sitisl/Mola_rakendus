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

using namespace std;

class FormMessenger : public QMainWindow
{
	Q_OBJECT

public:
	FormMessenger(QString userName, QString avatar, QWidget *parent = nullptr);
	~FormMessenger();

private:
	Ui::FormMessengerClass ui;
	QString m_userName;
	QString m_avatarPath;
	clientInfo_t client;

public slots:

private slots:
	void on_btnPicture_clicked();
	void on_btnSend_clicked();
	void on_btnEmoji_clicked();
	void insertEmoji(const QString& emoji);
	void on_lineEditMessage_textChanged();
	void receiveMessages();

};

