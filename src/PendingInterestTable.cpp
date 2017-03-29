#include"../header/PendingInterestTable.h"
#include"../header/global.h"

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

bool PendingInterestTable::isExist(ContentName name) const
{
	return (pit.count(name) == 1) ;
}

void PendingInterestTable::remove(ContentName name)
{
	pit.erase(name) ;
}

//由哪个接口转发要由调用者(所属节点)决定.
bool PendingInterestTable::add(ContentName name , Interface interface)
{
	if(!isExist(name) && capacity == pit.size())
	{
		return false ;
	}
	pit[name].insert(interface) ;
	return true ;
}
