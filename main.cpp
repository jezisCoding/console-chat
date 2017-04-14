#include <QCoreApplication>
#include <QDebug>
#include <iostream>

#include "myserver.h"
#include "myclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    char choice;
    do{
        qDebug() << "*1-Host server / 2-Connect to server*";
        std::cin >> choice;
    } while(choice != '1' && choice != '2');

    MyClient *instance;
    if(choice == '1') {
        instance = new MyServer;
        instance->run();
    } else {
        instance = new MyClient;
        instance->run();
    }

    int returned = a.exec();
    delete instance;
    std::cout << std::endl;
    return returned;
}
