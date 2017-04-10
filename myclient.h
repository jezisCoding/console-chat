#ifndef QCLIENT_H
#define QCLIENT_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QtConcurrent/QtConcurrentRun>

#include <string>
#include <iostream>

class MyClient : public QObject
{
    Q_OBJECT
public:
    explicit MyClient(QObject *parent = 0);

    virtual void run();

signals:
    void messageWritten();

protected:
    QTcpSocket *cliSocket;
    std::string message;
    QMutex mutex;

public slots:
    void createMsgThread();
    void disconnected();
    void connected();
    void readyRead();
    void writeMsg();
    void getMsg();
    void exitApplication();
    void error(QAbstractSocket::SocketError errora);
};

#endif // QCLIENT_H
