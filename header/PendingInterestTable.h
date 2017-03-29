#ifndef PENDING_INTEREST_TABLE
#define PENDING_INTEREST_TABLE

// #include"PendingInterestTableItem.h"
#include"ContentName.h"
#include"Interface.h"
#include<set>
#include<map>
#include<string>

using std::string ;
using std::map ;
using std::set ;

class PendingInterestTable
{
private :
	// vector<PendingInterestTableItem> pit ;
	map<ContentName , set<Interface> > pit ; 
	unsigned capacity ;//表示最多可以保存的item数
public :
	PendingInterestTable() ;
	PendingInterestTable(unsigned capacity) ;
	bool setCapacity(unsigned new_capacity) ;
	unsigned getCapacity() const { return capacity ; }
	unsigned getItemQuantity() const { return pit.size() ; }
	set<Interface> getInterfaces(ContentName name) ;
	bool isExist(ContentName name) const ;
	void remove(ContentName name) ;
	bool add(ContentName name , Interface interface) ;
} ;

#endif