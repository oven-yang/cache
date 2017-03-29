#ifndef FORWARDING_INTEREST_BASE_H
#define FORWARDING_INTEREST_BASE_H

#include"ContentName.h"
// #include"ForwardingInterestBaseItem.h"
#include"InterestPacket.h"
#include"Interface.h"
#include<map>
#include<set>

using std::map ;
using std::set ;

class ForwardingInterestBase
{
private :
	// vector<ForwardingInterestBaseItem> fib ;
	map<ContentName , set<Interface> > fib ;
	unsigned capacity ;
	unsigned item_quantity ;
public :
	ForwardingInterestBase() ;
	ForwardingInterestBase(unsigned capacity) ;
	unsigned getCapacity() const { return capacity ; }
	unsigned getItemQuantity() const { return item_quantity ; }
	bool setCapacity(unsigned new_capacity) ;
	bool add(InterestPacket interest_packet , Interface interface) ;
 	bool add(ContentName name , Interface interface) ;
	void remove(ContentName name) ;
	//调用前要定义一Interface容器来保存返回值.
	set<Interface> getInterfaces(ContentName name) ;
	bool isExist(ContentName name) const ;
	// bool isItemEmpty(ContentName name) ;
} ;

#endif
