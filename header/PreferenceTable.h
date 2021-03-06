#ifndef PREFERENCE_TABLE_H
#define PREFERENCE_TABLE_H

#include"global.h"
#include"ContentName.h"
#include<map>

using std::map ;

class PreferenceTable
{
private :
    map<ContentType , unsigned> send_table ;
    unsigned sum ;
    unsigned capacity ;
public :
    typedef map<ContentType , unsigned>::iterator iterator ;
    PreferenceTable() ;
    PreferenceTable(unsigned capacity) ;
    unsigned getQuantity(ContentName name) ;
    double getRate(ContentName name) ;
    unsigned updateQuantity(ContentName name) ;
    bool setCapacity(unsigned new_capacity) ;
    void clear() ;
    iterator begin() { return send_table.begin() ; }
    iterator end() { return send_table.end() ; }
} ;

#endif