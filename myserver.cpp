#include "myserver.h"

MyServer::MyServer(QObject *parent) : MyClient(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

MyServer::~MyServer()
{

}   //abstraktna nadtrieda ser a cli
    //kedze client socket sa na newConn nahradi tak nebude mat connecty

void MyServer::run()
{
    /*std::string host;
    int port;

    std::printf("Host address: ");
    std::cin >> host;
    std::printf("Host port: ");
    std::cin >> port;
    std::printf("Your nickname: ");
    std::cin >> nickname;
    std::cin.sync();*/    //flush buffer so getline doesnt skip input after cin >>...
    nickname = "server1";

    if(!server->listen(QHostAddress("localhost"), 1337/*QHostAddress(host.c_str()), port)*/)){
        qDebug() << "*Not listening!*";
        qDebug() << "*Error: " << server->errorString() << "*";
    } else {
        qDebug() << "*Listening!*";
    }
}

void MyServer::exitApplication()
{
    if(message == "quit"){
        broadcastMsg("Server: Bye bye!");
        qDebug() << "quitting MC";
        //say bye to clients
        //close server
        //close sockets
        QCoreApplication::quit();
    }
}

void MyServer::newConnection()
{
    cliSocks.append(server->nextPendingConnection());
    qDebug() << "*Connection accepted!*";

    connect(*(cliSocks.end()-1), SIGNAL(disconnected()), this, SLOT(clientDcd()));
    connect(*(cliSocks.end()-1), SIGNAL(readyRead()), this, SLOT(readyRead()));
    //connect(*(cliSocks.end()-1), SIGNAL(connected()), this, SLOT(connected()));
    connect(*(cliSocks.end()-1), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    (*(cliSocks.end()-1))->write("Hi Client!");
    (*(cliSocks.end()-1))->flush();
    (*(cliSocks.end()-1))->waitForBytesWritten();

    if(cliSocks.size() == 1) createMsgThread();
}

void MyServer::clientDcd()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "*A client disconnected!*";
    cliSocks.removeOne(client);
    //delete cliSocks.indexOf(sender());
    //cliSocks.remove(indexOf(sender()));
}

void MyServer::readyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    QByteArray msg = client->readAll();

    qDebug() << msg;
    broadcastMsg(msg);
}

void MyServer::writeMsg()
{
    broadcastMsg(message.toUtf8());
}

void MyServer::error(QAbstractSocket::SocketError errora)
{
    qDebug() << "*Error: " << errora << "*";
}

void MyServer::broadcastMsg(QByteArray msg)
{
    //slot connections inherited and other trash
    for(auto socket : cliSocks){
        socket->write(msg);
        socket->flush();
        socket->waitForBytesWritten();
    }
}
