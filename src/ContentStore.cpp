#include"../header/ContentStore.h"
#include"../header/global.h"
#include<stdexcept>
#include<algorithm>
#include<cstdlib>
#include<ctime>

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

string ContentStore::add(DataPacket data , double cache_priority)//LRU LRU-random
{
	if(cache_strategy != "PPDS")
	{
		throw std::runtime_error("cs:cache strategy is " + cache_strategy + " while cs::add fun is for PPDS") ;
	}
	if(capacity < data.getSize())
	{
		return "fail" ;
	}
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

string ContentStore::add(DataPacket data)
{
	if(cache_strategy != "LRU" && cache_strategy != "LRU-random")
	{
		throw std::runtime_error("cs:cache strategy is " + cache_strategy + " while cs::add fun is for LRU") ;
	}
	if(cache_strategy == "LRU-random")
	{
		srand(time(0)) ;
		if(rand()%2 == 0)
		{
			return "fail" ;
		}
	}
	bool remove_data = false ;
	data.setHop(0) ;
	data.setFromCache(true) ;
	auto it = find(content_LRU.begin() , content_LRU.end() , data) ;
	if(it != content_LRU.end())
	{
		return "exist" ;
	}
	if(capacity < data.getSize())
	{
		return "fail" ;
	}
	while(capacity - data_size < data.getSize())
	{
		data_size -= content_LRU.front().getSize() ;
		content_LRU.pop_front() ;
		remove_data = true ;
	}
	data_size += data.getSize() ;
	content_LRU.push_back(data) ;
	return (remove_data ? "replace" : "cached") ;
}

void ContentStore::clear()
{
	data_size = 0 ;
	content.clear() ;
	content_LRU.clear() ;
}

void ContentStore::remove(ContentName name)
{
	if(cache_strategy == "PPDS")
	{
		for(list<pair<DataPacket , double> >::iterator it = content.begin() ; it != content.end() ; ++it)
		{
			if(it->first.getName() == name)
			{
				content.erase(it) ;
				return ;
			}
		}
		return ;
	}
	else if(cache_strategy == "LRU" || cache_strategy == "LRU-random")
	{
		for(auto it = content_LRU.begin() ; it != content_LRU.end() ; ++it)
		{
			if(it->getName() == name)
			{
				it = content_LRU.erase(it) ;
				return ;
			}
		}
		return ;
	}
	else if(cache_strategy == "no-cache")
	{
		//no cached content
		return ;
	}
	throw std::runtime_error("cache strategy not defined") ;
}

unsigned ContentStore::setCapacity(unsigned new_capacity)
{
	return ((data_size > new_capacity) ? -1 : (capacity = new_capacity)) ;
}

bool ContentStore::isExist(ContentName name) const
{
	if(cache_strategy == "PPDS")
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
	else if(cache_strategy == "LRU" || cache_strategy == "LRU-random")
	{
		for(auto it = content_LRU.begin() ; it != content_LRU.end() ; ++it)
		{
			if(it->getName() == name)
			{
				return true ;
			}
		}
		return false ;
	}
	else if(cache_strategy == "LRU-random")
	{
		for(auto it = content_LRU.begin() ; it != content_LRU.end() ; ++it)
		{
			if(it->getName() == name)
			{
				return true ;
			}
		}
		return false ;
	}
	else if(cache_strategy == "no-cache")
	{
		return false ;
	}
	throw std::runtime_error("cache strategy not defined") ;
}

DataPacket ContentStore::getDataPacket(ContentName name)
{
	if(cache_strategy == "PPDS")
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
	else if(cache_strategy == "LRU")
	{
		for(auto it = content_LRU.begin() ; it != content_LRU.end() ; ++it)
		{
			if(it->getName() == name)
			{
				content_LRU.splice(content_LRU.end() , content_LRU , it) ;
				return *it ;
			}
		}
		return DataPacket() ;
	}
	else if(cache_strategy == "LRU-random")
	{
		for(auto it = content_LRU.begin() ; it != content_LRU.end() ; ++it)
		{
			if(it->getName() == name)
			{
				content_LRU.splice(content_LRU.end() , content_LRU , it) ;
				return *it ;
			}
		}
		return DataPacket() ;
	}
	else if(cache_strategy == "no-cache")
	{
		return DataPacket() ;
	}
	throw std::runtime_error("cache strategy not defined") ;
}