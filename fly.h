#ifndef FLY_H
#define FLY_H
#include <QString>
#include <QThreadPool>
#include <QObject>
#include <QFile>
#include <QTextStream>
class CSVTable;

class Worker : public QObject, public QRunnable
{
    Q_OBJECT
private:
    CSVTable* _fTable;
    QString _name;
    QString _city;

public:
    Worker( CSVTable*, const QString& p, const QString& c );
signals:
    void ready( QString, int );
public slots:
    void run();

};

class Fly : public QObject
{
    Q_OBJECT
private:
    int _readyCount{0};
    int _rowCount{0};
    QFile _fileOut;
    QString _testFileName;
    QThreadPool _tPool;
    static const int _threadCount;
    static const QString _flightName;
    static const QString _departName;
    static const QString _outName;

public:
    Fly( const QString& );
    ~Fly();
    void calculate( );
signals:
    void done();
public slots:
    void onReady( QString, int );
};

#endif // FLY_H
