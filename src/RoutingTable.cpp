#include"../header/RoutingTable.h"

RoutingTable::RoutingTable()
{
    //
}

Interface* RoutingTable::getInterface(ContentName name)
{
    RoutingTableItem *p = &root ;
    vector<string> formed_name = name.getFormedName() ;
    vector<string>::iterator it = formed_name.begin() ;
    while(it != formed_name.end() && p->next.count(*it) == 1)
    {
        p = p->next[*it] ;
        ++it ;
    }
    return p->interface ;
}

void RoutingTable::add(ContentName name , Interface *interface)
{
    RoutingTableItem *p = &root ;
    vector<string> formed_name = name.getFormedName() ;
    vector<string>::iterator it = formed_name.begin() ;
    while(it != formed_name.end() &&  p->next.count(*it) == 1)
    {
        p = p->next[*it] ;
        ++it ;
    }
    while(formed_name.end() != it)
    {
        p = p->next[*it] = new RoutingTableItem() ;
        p->interface = interface ;
        ++it ;
    }
}
