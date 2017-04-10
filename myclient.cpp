#include "myclient.h"

MyClient::MyClient(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(messageWritten()), this, SLOT(exitApplication()),
            Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(messageWritten()), this, SLOT(createMsgThread()),
            Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(messageWritten()), this, SLOT(writeMsg()),
            Qt::ConnectionType::QueuedConnection);
}

MyClient::~MyClient()
{

}

void MyClient::run()
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
    nickname = "client1";

    cliSocket = new QTcpSocket(this);

    connect(cliSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(cliSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(cliSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(cliSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    cliSocket->connectToHost("localhost", 1337/*host.c_str(), port*/);
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
    qDebug() << cliSocket->readAll();
}

void MyClient::createMsgThread()
{
    QFuture<void> fvoid = QtConcurrent::run(this, &getMsg);
    //fvoid.waitForFinished();
}

void MyClient::writeMsg()
{
    cliSocket->write(nickname.toUtf8() + ": " + message.toUtf8());
    cliSocket->flush();
    cliSocket->waitForBytesWritten();
}

void MyClient::getMsg()
{
    QMutexLocker locker(&mutex);

    std::string msg;
    std::cin.sync();    //skipping again
    std::getline(std::cin, msg);
    message = QString(msg.c_str());

    //if quit exitapp
    //write
    //createmsgthread NOT, only getmsg?

    emit messageWritten();
}

void MyClient::exitApplication()
{
    if(message == "quit"){
        mutex.unlock();
        cliSocket->disconnectFromHost();
        QCoreApplication::quit();
    }
}

void MyClient::error(QAbstractSocket::SocketError errora)
{
    if(errora == QAbstractSocket::SocketError::RemoteHostClosedError){
        qDebug() << "*Host closed the connection!*";
        exitApplication();
    }

    else qDebug() << "*Error: " << errora << "*";
}
