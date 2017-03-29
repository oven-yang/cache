#ifndef POPULARITY_TABLE_H
#define POPULARITY_TABLE_H

#include"ContentName.h"
#include"global.h"
#include<map>

using std::map ;

class PopularityTable
{
private :
    map<ContentName , unsigned> receive_table ;
    unsigned sum ;
    unsigned capacity ;
public :
    typedef map<ContentName , unsigned>::iterator iterator ;
    PopularityTable() ;
    PopularityTable(unsigned capacity) ;
    unsigned getQuantity(ContentName name) ;
    double getRate(ContentName name) ;
    unsigned updateQuantity(ContentName name) ;
    bool setCapacity(unsigned new_capacity) ;
    void clear() ;
    iterator begin() { return receive_table.begin() ; }
    iterator end() { return receive_table.end() ; }
} ;

#endif