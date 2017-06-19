#include"../header/PendingInterestTable.h"
#include"../header/global.h"
#include<algorithm>

PendingInterestTable::PendingInterestTable() : capacity(PIT_DEFAULT_CAPACITY)
{
	//
}

PendingInterestTable::PendingInterestTable(unsigned capacity) : capacity(capacity)
{
	//
}

bool PendingInterestTable::setCapacity(unsigned new_capacity)
{
	if(new_capacity < pit.size())
	{
		return false ;
	}
	capacity = new_capacity ;
	return true ;
}

set<Interface> PendingInterestTable::getInterfaces(ContentName name)
{
	if(pit.count(name) == 1)
	{
		return pit[name] ;
	}
	return set<Interface>() ;
}

bool PendingInterestTable::isExist(ContentName name , Interface interface) const
{
	auto it = pit.find(name) ;
	return it != pit.cend() && it->second.find(interface) != it->second.end() ;
}

bool PendingInterestTable::isExist(ContentName name) const
{
	return pit.find(name) != pit.cend() ;
}

void PendingInterestTable::remove(ContentName name)
{
	pit.erase(name) ;
}

//由哪个接口转发要由调用者(所属节点)决定.
bool PendingInterestTable::add(ContentName name , Interface interface)
{
	pit[name].insert(interface) ;
	return true ;
}
