#ifndef ROUTING_TABLE_ITEM_H
#define ROUTING_TABLE_ITEM_H

#include"Interface.h"
#include<map>
#include<string>

using std::map ;
using std::string ;

class RoutingTableItem
{
    friend class RoutingTable ;
    map<string , RoutingTableItem*> next ;
    Interface *interface ;//合成析构函数不删除指针指向的对象
    
    RoutingTableItem() ;
    RoutingTableItem(const RoutingTableItem &item) ;
    RoutingTableItem* copy(RoutingTableItem* item) ;
	~RoutingTableItem() ;
} ;

#endif
