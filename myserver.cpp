#include "myserver.h"

MyServer::MyServer(QObject *parent) : MyClient(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void MyServer::run()
{
    std::string nick;
    std::printf("Your nickname: ");
    std::cin >> nick;
    nickname = QString(nick.c_str());
    std::cin.sync();    //flush buffer so getline doesnt skip input after cin >>...

    if(!server->listen(QHostAddress("localhost"), 1337)){
        qDebug() << "*Not listening!*";
        qDebug() << "*Error: " << server->errorString() << "*";
    } else {
        qDebug() << "*Listening!*";
        qDebug() << "*Waiting for clients!*";
    }
}

void MyServer::exitApplication()
{
    if(message == "quit"){
        broadcastMsg("Bye bye!", nullptr);
        QCoreApplication::quit();
    }
}

void MyServer::newConnection()
{
    cliSocks.append(server->nextPendingConnection());
    qDebug() << "*Connection accepted!*";

    connect(*(cliSocks.end()-1), SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(*(cliSocks.end()-1), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    message = "Hi Client!";
    writeMsg();

    if(cliSocks.size() == 1) createMsgThread();
}

void MyServer::readyRead()
{
    QTcpSocket* sSender = qobject_cast<QTcpSocket*>(sender());
    QByteArray msg = sSender->readAll();

    qDebug() << msg;
    broadcastMsg(msg, sSender);
}

void MyServer::writeMsg()
{
    broadcastMsg(nickname.toUtf8() + ": " + message.toUtf8(), nullptr);
}

void MyServer::error(QAbstractSocket::SocketError errora)
{
    if(errora == QAbstractSocket::SocketError::RemoteHostClosedError){
        QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
        qDebug() << "*A client disconnected!*";
        cliSocks.removeOne(client);
        broadcastMsg("A client disconnected!", nullptr);
    }
    else qDebug() << "*Error: " << errora << "*";
}

void MyServer::broadcastMsg(QByteArray msg, QTcpSocket *except)
{
    for(auto socket : cliSocks){
        if(!(socket == except)){  //skips the sender
            socket->write(msg);
            socket->flush();
        }
    }
}
