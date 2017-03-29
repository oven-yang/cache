#include"../header/ForwardingInterestBase.h"
#include"../header/Interface.h"

ForwardingInterestBase::ForwardingInterestBase() : capacity(FIB_DEFAULT_CAPACITY) , item_quantity(0)
{
	//
}

ForwardingInterestBase::ForwardingInterestBase(unsigned capacity) : capacity(capacity) , item_quantity(0)
{
	//
}

bool ForwardingInterestBase::add(InterestPacket interest_packet , Interface interface)
{
	return add(interest_packet.getName() , interface) ;
}

bool ForwardingInterestBase::add(ContentName name , Interface interface)
{
	if(!isExist(name) && capacity == item_quantity)
	{
		return false ;
	}
	fib[name].insert(interface) ;
	++item_quantity ;
	return true ;
}

void ForwardingInterestBase::remove(ContentName name)
{
	fib.erase(name) ;
	--item_quantity ;
}

set<Interface> ForwardingInterestBase::getInterfaces(ContentName name)
{
	if(fib.count(name) == 1)
	{
		return fib[name] ;
	}
	set<Interface> temp ;
	return temp ;
}

bool ForwardingInterestBase::isExist(ContentName name) const
{
	return (fib.count(name) == 1) ;
}