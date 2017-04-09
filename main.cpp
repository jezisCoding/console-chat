#include <QCoreApplication>
#include <QDebug>
#include <iostream>

#include "myserver.h"
#include "myclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int choice;
    do{
        qDebug() << "1-Host server / 2-Connect to server";
        std::cin >> choice;
    } while(choice != 1 && choice != 2);

    if(choice == 1) {
        MyServer ms;
        ms.run();
    } else {
        MyClient mc;
        mc.run();
    }

    return a.exec();
}

/* choose to host or connect
 * disconnect/exit fluently
 * more simultaneous clients, all get messages on board
 * abstraktna nadtrieda ser a cli
 * TESTED ONLY ON LAN, reccomended input localhost:1337
 */
