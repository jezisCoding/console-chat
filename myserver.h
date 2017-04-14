#ifndef QSERVER_H
#define QSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtConcurrent/QtConcurrentRun>
#include <QCoreApplication>

#include <string>
#include <iostream>

#include "myclient.h"

class MyServer : public MyClient
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    void run();

private:
    QTcpServer *server;
    QVector<QTcpSocket*> cliSocks;

signals:

public slots:
    void exitApplication();
    void newConnection();
    void readyRead();
    void writeMsg();
    void error(QAbstractSocket::SocketError errora);
    void broadcastMsg(QByteArray msg, QTcpSocket *except);
};

#endif // QSERVER_H
