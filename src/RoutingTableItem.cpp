#include"../header/RoutingTableItem.h"

RoutingTableItem::RoutingTableItem() : interface(0)
{
    //
}

RoutingTableItem* RoutingTableItem::copy(RoutingTableItem* item)
{
	RoutingTableItem* p = new RoutingTableItem() ;
	p->interface = item->interface ;
	for(map<string , RoutingTableItem*>::const_iterator it = item->next.begin() ; it != item->next.end() ; ++it)
	{
		next[it->first] = copy(item->next[it->first]) ;
	}
	return p ;
}

RoutingTableItem::RoutingTableItem(const RoutingTableItem &item)
{
	interface = item.interface ;
	for(map<string , RoutingTableItem*>::const_iterator it = item.next.begin() ; it != item.next.end() ; ++it)
	{
		next[it->first] = copy(it->second) ;
	}
}

RoutingTableItem::~RoutingTableItem()
{
	for(map<string , RoutingTableItem*>::iterator it = next.begin() ; it != next.end() ; ++it)
	{
		delete it->second ;
		it->second = 0 ;
	}
}
