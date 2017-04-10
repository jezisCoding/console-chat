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
    ~MyClient();
    virtual void run();

protected:
    QString message;
    QString nickname;
    QMutex mutex;

private:
    QTcpSocket *cliSocket;

signals:
    void messageWritten();

public slots:
    virtual void readyRead();
    void createMsgThread();
    virtual void writeMsg();
    void getMsg();
    virtual void exitApplication();
    void error(QAbstractSocket::SocketError errora);

private slots:
    void disconnected();
    void connected();
};

#endif // QCLIENT_H
