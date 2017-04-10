#include "myclient.h"

MyClient::MyClient(QObject *parent) : QObject(parent)
{
    cliSocket = new QTcpSocket(this);

    connect(this, SIGNAL(messageWritten()), this, SLOT(exitApplication()),
            Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(messageWritten()), this, SLOT(writeMsg()),
            Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(messageWritten()), this, SLOT(createMsgThread()),
            Qt::ConnectionType::QueuedConnection);
}

void MyClient::run()
{
    /*std::string host;
    int port;

    std::printf("Host address: ");
    std::cin >> host;
    std::printf("Host port: ");
    std::cin >> port;
    std::cin.sync();*/    //flush buffer so getline doesnt skip input after cin >>...

    connect(cliSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(cliSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(cliSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(cliSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    cliSocket->connectToHost("localhost", 1337/*host.c_str(), port*/);
}

void MyClient::createMsgThread()
{
    QFuture<void> fvoid = QtConcurrent::run(this, &getMsg);
    //fvoid.waitForFinished();
}

void MyClient::getMsg()
{
    QMutexLocker locker(&mutex);

    std::getline(std::cin, message);

    emit messageWritten();
}

void MyClient::exitApplication()
{
    if(message == "quit"){
        cliSocket->close();
        QCoreApplication::quit();
    }
}

void MyClient::connected()
{
    qDebug() << "*Connected!*";

    cliSocket->write("Hi Server!");
    cliSocket->flush();
    cliSocket->waitForBytesWritten();

    createMsgThread();
}

void MyClient::disconnected()
{
    qDebug() << "*Disconnected!*";
}

void MyClient::readyRead()
{
    qDebug() << "Other: " << cliSocket->readAll();
}

void MyClient::writeMsg()
{
    cliSocket->write(message.c_str());
    cliSocket->flush();
    cliSocket->waitForBytesWritten();
}

void MyClient::error(QAbstractSocket::SocketError errora)
{
    qDebug() << "*Error: " << errora << "*";
}
