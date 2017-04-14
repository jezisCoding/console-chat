#include "myclient.h"

MyClient::MyClient(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(messageWritten()), this, SLOT(writeMsg()),
            Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(messageWritten()), this, SLOT(createMsgThread()),
            Qt::ConnectionType::QueuedConnection);
}

void MyClient::run()
{
    std::string host;
    std::string nick;

    std::printf("Host address: ");
    std::cin >> host;
    std::printf("Your nickname: ");
    std::cin >> nick;

    nickname = QString(nick.c_str());
    std::cin.sync();    //flush buffer so getline doesnt skip input after cin >>...

    cliSocket = new QTcpSocket(this);

    connect(cliSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(cliSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(cliSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(cliSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    cliSocket->connectToHost(host.c_str(), 1337);
}

void MyClient::connected()
{
    qDebug() << "*Connected!*";

    message = "Hi Server!";
    writeMsg();

    createMsgThread();
}

void MyClient::disconnected()
{
    qDebug() << "*Disconnected!*";
}

void MyClient::readyRead()
{
    qDebug() << cliSocket->readAll();
}

void MyClient::createMsgThread()
{
    if(message == "quit") exitApplication();
    else fvoid = QtConcurrent::run(this, &getMsg);
}

void MyClient::writeMsg()
{
    cliSocket->write(nickname.toUtf8() + ": " + message.toUtf8());
    cliSocket->flush();
}

void MyClient::getMsg()
{
    QMutexLocker locker(&mutex);

    std::string msg;
    std::cin.sync();    //skipping again
    std::getline(std::cin, msg);
    message = QString(msg.c_str());

    emit messageWritten();
}

void MyClient::exitApplication()
{
    fvoid.cancel();
    QCoreApplication::quit();
}

void MyClient::error(QAbstractSocket::SocketError errora)
{
    if(errora == QAbstractSocket::SocketError::RemoteHostClosedError){
        qDebug() << "*Host closed the connection!*";
    } else if(errora == QAbstractSocket::ConnectionRefusedError){
        qDebug() << "*Error: " << errora << "*";
        run();
    }
    else qDebug() << "*Error: " << errora << "*";
}
