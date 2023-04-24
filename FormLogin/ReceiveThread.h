#pragma once

#include <QThread>
#include "initClient.h"
#include <QImage>
#include <QTime>

class ReceiveThread  : public QThread
{
	Q_OBJECT

public:
    ReceiveThread(clientInfo_t client, QObject* parent = nullptr);
    ~ReceiveThread();

signals:
    void usersReceived(QString users);
    void messageReceived(QString message, QImage avatar);
    void imageReceived(QByteArray image);

protected:
    void run() override;

private:
    clientInfo_t client;
    
};

