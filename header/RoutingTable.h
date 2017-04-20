#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H

#include"RoutingTableItem.h"
#include"ContentName.h"

class RoutingTable
{
private :
    friend void init() ;
    friend int main(int , char* []) ;
    void add(ContentName name , Interface *interface) ;
	void clear(RoutingTableItem* p) ;
    RoutingTableItem root ;
public :
    RoutingTable() ;
    Interface* getInterface(ContentName name) ;
} ;

#endif
