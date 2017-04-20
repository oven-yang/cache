#ifndef CONTENT_STORE_ITEM_H
#define CONTENT_STORE_ITEM_H

#include<string>
#include<utility>
#include<list>
#include<queue>
#include"../header/ContentName.h"
#include"../header/DataPacket.h"

// using std::map ;

// class ContentStore
// {
// private :
// 	friend void updateCachePriority() ;
// 	map<DataPacket , double> content ;
// 	unsigned capacity ;
// 	unsigned data_size ;
// public :
// 	ContentStore() ;
// 	ContentStore(unsigned capacity) ;
// 	bool add(DataPacket data , double cache_priority) ;
// 	void clear() ;
// 	void remove(ContentName name) ;
// 	unsigned setCapacity(unsigned new_capacity) ;//return the capacity after the function function
// 	unsigned getCapacity() const { return capacity ; }
// 	unsigned getDataSize() const { return data_size ; }
// 	unsigned getRemainedSize() const { return capacity - data_size ; }
// 	bool isExist(ContentName name) const ;
// 	DataPacket getDataPacket(ContentName name) const ;
// 	void cache(DataPacket packet , double priority) ;
// } ;

using std::list ;
using std::queue ;
using std::pair ;
using std::make_pair ;

class ContentStore
{
private :
	friend class Node ;
	list<pair<DataPacket , double> > content ;
	list<DataPacket> content_LRU ;
	unsigned capacity ;
	unsigned data_size ;
	void insert(DataPacket data , double cache_priority) ;
public :
	ContentStore() ;
	ContentStore(unsigned capacity) ;

	//提供默认值是为了当有的缓存策略不是基于缓存优先级时，就不会提供这个参数
	string add(DataPacket data , double cache_priority) ;
	string add(DataPacket data) ;

	void clear() ;
	void remove(ContentName name) ;
	unsigned setCapacity(unsigned new_capacity) ;//return the capacity after the function function
	unsigned getCapacity() const { return capacity ; }
	unsigned getDataSize() const { return data_size ; }
	unsigned getRemainedSize() const { return capacity - data_size ; }
	bool isExist(ContentName name) const ;
	DataPacket getDataPacket(ContentName name) ;
	// void cache(DataPacket packet , double priority) ;
} ;

#endif // !CONTENT_STORE_ITEM_H