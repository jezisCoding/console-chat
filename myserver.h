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

signals:

public slots:
    void exitApplication();
    void newConnection();
    void clientDcd();
};

#endif // QSERVER_H
