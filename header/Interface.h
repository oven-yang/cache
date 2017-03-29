/*
 * 默认所有接口均关闭,初始化时记得打开接口.
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include"global.h"
#include<string>

using namespace std ;

class Interface
{
private :
	unsigned id ;//id == 0 的接口用于和接口所在节点通信.
	string name ;
	InterfaceState state ;
public :
	Interface() ;
	Interface(unsigned id) ;
	unsigned getId() const { return id ; }
	InterfaceState getState() const { return state ; }
	string getName() const { return name ; }
	void setId(unsigned new_id) { id = new_id ; }
	void setState(InterfaceState new_state){ state = new_state ; }
	void setName(string new_name){ name = new_name ; }
	inline bool operator==(const Interface interface1){ return id == interface1.getId() ; }
	inline bool operator!=(const Interface interface1){ return id != interface1.getId() ; }
	inline Interface& operator=(const Interface interface1)
	{
		id = interface1.getId() ;
		name = interface1.getName() ;
		state = interface1.getState() ;
		return *this ;
	}
	inline bool operator<(const Interface& interafce1) const
	{
		return id < interafce1.getId() ;
	}
} ;

#endif
