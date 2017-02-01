#include "csvtable.h"
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QStringList>
#include <cassert>
#include <QHash>
#include <QDebug>

CSVTable::CSVTable()
{
}


CSVTable::~CSVTable()
{

}

bool CSVTable::load( const QString &  fileName )
{
    QFile file( fileName );

    if( !file.open( QIODevice::ReadOnly ) )
        return false;
    QTextStream stream( &file );
    while( !stream.atEnd() )
    {
       QStringList row;
       row = stream.readLine().split( QRegExp (",") );
       if( _columCount == 0 )
            _columCount = row.size();

       if( _columCount == row.size() )
           _table.append( row );

       else if( _columCount < row.size() )
            _table.append( row.mid( 0, _columCount ) );
       else if(  _columCount > row.size() )
       {
           int sub = _columCount - row.size();
            while( sub-- )
                row.append("");
            _table.append( row );
       }
       ++_rowCount;
    }
    _tableIndexes.resize( _columCount );
    return true;
}



QStringList CSVTable::getRow( const int row ) const
{
    if( row < 0 || row >= _rowCount )
        assert( "Out of range" );
    return _table[row];
}

void CSVTable::setRow( const int row , const QStringList & data )
{
    if( row < 0 || row >= _rowCount )
        assert( "Out of range" );
    _table[row] = data;
}

QString CSVTable::getItem( const int row, const int column ) const
{
    if( ( row < 0 ) || ( column < 0 ) || ( row >= _rowCount ) || ( _table[row].size() >= column ) )
        assert( "Out of range" );
    return _table[row][column];
}

int CSVTable::getRowCount() const
{
    return _rowCount;
}

int CSVTable::getColumnCount() const
{
    return _columCount;
}

void CSVTable::_hashJoin( const int column, const int jcolumn, CSVTable * jtable )
{
    QHash<QString, int> hash;
    for( int i = 0; i < jtable->getRowCount(); ++i )
        hash[jtable->getItem( i, jcolumn )] = i;

    for( int i = 0; i < getRowCount(); ++i )
    {
        if( hash.contains( getItem( i, column ) ) )
        {
            int fRow { hash[ getItem( i, column ) ] };
            QStringList r = this->getRow( i );

            for( int y = 0; y < jtable->getRow( fRow ).size(); ++y )
                if( y != jcolumn )
                    r.append( jtable->getItem( fRow, y ) );
            this->setRow( i, r );
        }
    }
    _columCount += ( jtable->getColumnCount() - 1);
    _tableIndexes.resize( _columCount );
}

void CSVTable::createIndex( const int column )
{

}

void CSVTable::createIndex( const int column , const QRegExp &  re )
{
    QHash<QString, QSet<int> > hash;
    QString item, modifyItem;
    int pos{-1};
    for( int i = 0; i < _rowCount; ++i )
    {
        item = _table[i][column];
        if( ( pos = re.indexIn( item ) ) != -1 )
        {
            modifyItem = item.mid( pos, re.matchedLength() );
            hash[modifyItem].insert( i );
        }
    }
    _tableIndexes[column] = hash;
}

QSet<int> CSVTable::find(  const int column, const QString & value )
{
    if( _tableIndexes[column].size() == 0 )
        return QSet<int>();
    return _tableIndexes[column][value];
}


void CSVTable::join(const int column, const int jcolumn, CSVTable *jtable )
{

    _hashJoin( column, jcolumn, jtable );
}

void CSVTable::print() const
{
    for(  int i = 0; i < getRowCount(); ++i )
        qDebug()<<getRow( i );
}


