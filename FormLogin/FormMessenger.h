#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <QMainWindow>
#include "ui_FormMessenger.h"
#include "initClient.h"
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QPixMap>
using namespace std;

class FormMessenger : public QMainWindow
{
	Q_OBJECT

public:
	FormMessenger(QString userName, QWidget *parent = nullptr);
	~FormMessenger();

private:
	Ui::FormMessengerClass ui;
	QString m_userName;
	clientInfo_t client;

public slots:

private slots:
	void on_btnPicture_clicked();
	void on_btnSend_clicked();
	void on_btnEmoji_clicked();
};

