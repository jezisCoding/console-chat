#include "myserver.h"

MyServer::MyServer(QObject *parent) : MyClient(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}   //abstraktna nadtrieda ser a cli
    //kedze client socket sa na newConn nahradi tak nebude mat connecty

void MyServer::run()
{
    std::string host;
    int port;

    std::printf("Host address: ");
    std::cin >> host;
    std::printf("Host port: ");
    std::cin >> port;
    std::cin.sync();    //flush buffer so getline doesnt skip input after cin >>...

    if(!server->listen(QHostAddress(host.c_str()), port)){
        qDebug() << "*Not listening!*";
    } else {
        qDebug() << "*Listening!*";
    }
}

void MyServer::exitApplication()
{
    if(message == "quit"){
        cliSocket->close();
        QCoreApplication::quit();
    }
    //say bye to clients
}

void MyServer::newConnection()
{
    cliSocket = server->nextPendingConnection();
    qDebug() << "*Connection accepted!*";
    connect(cliSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(cliSocket, SIGNAL(disconnected()), this, SLOT(clientDcd()));
    connect(cliSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(cliSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    cliSocket->write("Hi Client!");
    cliSocket->flush();
    cliSocket->waitForBytesWritten();

    createMsgThread();
}

void MyServer::clientDcd()
{
    qDebug() << "*A client disconnected!*";
}
