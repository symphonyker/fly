#ifndef CSVTABLE_H
#define CSVTABLE_H

#include <QString>
#include <QList>
#include <QHash>
#include <QVector>
#include <QSet>
#include <QRegExp>
#include <QSharedPointer>


class CSVTable
{
private:
    QList< QStringList > _table;
    QVector< QHash<QString, QSet<int> > > _tableIndexes;
    int _rowCount{0};
    int _columCount{0};
    void _hashJoin( const int , const int ,CSVTable* );
public:
    CSVTable();
    virtual ~CSVTable();
    bool load( const QString& );
    QStringList getRow( const int ) const;
    void setRow( const int, const QStringList& );
    QString getItem( const int, const int ) const;
    void sortBy( const int );
    void join(  const int, const int, CSVTable* );
    int getRowCount() const;
    int getColumnCount() const;
    void createIndex( const int );
    void createIndex( const int, const QRegExp& );
    QSet<int> find( const int, const QString& );
    void print() const;
};

#endif // CSVTABLE_H
