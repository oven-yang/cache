#ifndef DATA_PACKET_H
#define DATA_PACKET_H

#include"ContentName.h"
#include"global.h"
#include<string>

class DataPacket
{
private :
	ContentName name ;
	unsigned size ;
	PacketType packetType ;
	bool fromCache ;
	int hop ;
public :
	DataPacket() ;
	DataPacket(ContentName name) ;//用来测试
	DataPacket(ContentName name , unsigned size , bool from_cache) ;
	DataPacket(string name , unsigned size , bool from_cache) ;
	PacketType getPacketType() const { return packetType ; }
	ContentName getName() const { return name ; }
	unsigned getSize() const { return size ; }
	int getHop() const { return hop ; }
	void setSize(unsigned new_size){ size = new_size ; }
	void setHop(int new_hop) { this->hop = new_hop ; }
	void setFromCache(bool flag) { fromCache = flag ; }
	void riseHop(){ ++hop ; }
	string isFromCache(){ return fromCache ? "cache" : "disk" ; }
	inline bool operator==(const DataPacket &data1)
	{
		return name == data1.getName() ;
	}
	inline bool operator!=(const DataPacket &data1) const
	{
		return name != data1.getName() ;
	}
	inline DataPacket& operator=(const DataPacket &data1)
	{
		name = data1.getName() ;
		size = data1.getSize() ;
		return *this ;
	}
	inline bool operator<(const DataPacket &data1) const
	{
		return name < data1.getName() ;
	}
} ;

#endif
