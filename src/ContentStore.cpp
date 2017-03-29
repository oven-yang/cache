#include"../header/ContentStore.h"
#include"../header/global.h"

ContentStore::ContentStore() : capacity(CS_DEFAULT_CAPACITY) , data_size(0)
{
	//
}

ContentStore::ContentStore(unsigned capacity) : capacity(capacity) , data_size(0)
{
	//
}

void ContentStore::insert(DataPacket data , double cache_priority)
{
	list<pair<DataPacket , double> >::iterator it(content.begin()) ;
	while(it != content.end() && it->second <= cache_priority)
	{
		++it ;
	}
	content.insert(it , make_pair(data , cache_priority)) ;
}

string ContentStore::add(DataPacket data , double cache_priority)
{
	list<pair<DataPacket , double> >::iterator it(content.begin()) , remove_it(content.end()) ;
	unsigned data_size_sum(0) ;
	data.setHop(0) ;
	data.setFromCache(true) ;
	for( ; it != content.end() ; ++it)
	{
		if(it->first.getName() == data.getName())//该内容已经存在于CS中
		{
			return "exist" ;
		}
		if(it->second < cache_priority && data_size_sum < data.getSize())
		{
			if((data_size_sum += it->first.getSize()) >= data.getSize())
			{
				remove_it = it ;
			}
		}
	}
	if(capacity - data_size >= data.getSize())
	{
		data_size += data.getSize() ;
		insert(data , cache_priority) ;
		return "cached" ;
	}
	else if(data_size_sum >= data.getSize() && remove_it != content.end())
	{
		data_size_sum -= (data_size_sum - data.getSize()) ;
		content.erase(content.begin() , ++remove_it) ;
		insert(data , cache_priority) ;
		return "replace" ;
	}
	return "fail" ;
}

void ContentStore::clear()
{
	data_size = 0 ;
	content.clear() ;
}

void ContentStore::remove(ContentName name)
{
	for(list<pair<DataPacket , double> >::iterator it = content.begin() ; it != content.end() ; ++it)
	{
		if(it->first.getName() == name)
		{
			content.erase(it) ;
			return ;
		}
	}
}

unsigned ContentStore::setCapacity(unsigned new_capacity)
{
	return ((data_size > new_capacity) ? -1 : (capacity = new_capacity)) ;
}

bool ContentStore::isExist(ContentName name) const
{
	for(list<pair<DataPacket , double> >::const_iterator it = content.begin() ; it != content.end() ; ++it)
	{
		if(it->first.getName() == name)
		{
			return true ;
		}
	}
	return false ;
}

DataPacket ContentStore::getDataPacket(ContentName name) const
{
	for(list<pair<DataPacket , double> >::const_iterator it = content.begin() ; it != content.end() ; ++it)
	{
		if(it->first.getName() == name)
		{
			return it->first ;
		}
	}
	//未找到,返回假包.
	//打印错误日志.
	return DataPacket() ;
}