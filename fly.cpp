#include "fly.h"
#include "flights.h"
#include "departs.h"
#include <unistd.h>
#include <QStringList>
#include <QDataStream>
#include <QDebug>

const int Fly::_threadCount = 3;
const QString Fly::_departName = "departs.csv";
const QString Fly::_flightName = "flights.csv";
const QString Fly::_outName = "out.csv";

Fly::Fly( const QString & fileName )
    :_testFileName( fileName )
{
    _fileOut.setFileName( _outName );
    _fileOut.open( QIODevice::WriteOnly );
    _tPool.setMaxThreadCount( _threadCount );
}
Fly::~Fly()
{
    _fileOut.close();
    _tPool.waitForDone();
}

void Fly::calculate( )
{
    Flights fly;
    if( !fly.load( _flightName ) )
    {
        qDebug()<<"Cant load flights";
        return;
    }
    Departs dep;

    if( !dep.load( _departName ) )
    {
        qDebug()<<"Cant load departs";
        return;
    }
    CSVTable test;
    if( !test.load( _testFileName ) )
    {
        qDebug()<<"Cant load test";
        return;
    }

    fly.join( 0, 0, dynamic_cast<CSVTable*> ( &dep ) );
    fly.createIndex( 2, QRegExp( QString::fromLocal8Bit( "[А-Я]\\w+" ) ) );
    fly.createIndex( 3, QRegExp( QString::fromLocal8Bit( "\\w+" ) ) );

    _readyCount = 0;
    _rowCount = test.getRowCount();
    for( int i = 0; i < _rowCount; ++i )
    {
        Worker *w =  new Worker( dynamic_cast<CSVTable*> ( &fly ), test.getItem( i, 0 ), test.getItem( i, 1 )  );
        connect( w, SIGNAL(ready(QString,int)), this, SLOT(onReady(QString,int)), Qt::QueuedConnection );
        _tPool.start( w );
    }
}



void Fly::onReady( QString name, int count )
{

;
    QTextStream outStream( &_fileOut );
    outStream<<name;
    outStream<<",";
    outStream<<count;
    outStream<<"\n";
    ++_readyCount;
    if( _readyCount == _rowCount )
        emit done();
}



Worker::Worker( CSVTable * t, const QString &p, const QString &c)
    :_fTable( t ), _name( p ), _city( c )
{

}

void Worker::run()
{
    QSet<int> p = _fTable->find( 2, _name );
    QSet<int> c = _fTable->find( 3, _city );
    emit ready( _name, p.intersect( c ).size() );
}

