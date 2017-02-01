#include <QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QObject>
#include "fly.h"


int main(int argc, char *argv[])
{
    if( argc != 2 )
    {
        qDebug()<<"Invalid parametrs";
        return 1;
    }
    QCoreApplication a(argc, argv);
    Fly fly( argv[1] );
    fly.calculate();
    QObject::connect( &fly, SIGNAL(done()), &a, SLOT( quit() ) );
    return a.exec();
}
