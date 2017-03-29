#include"../header/DataPacket.h"

//表示无用包,假包,用于寻找数据包未找到时作为返回值.
DataPacket::DataPacket() : name(ContentName()) , size(0) , packetType(DataPacketType) ,
						fromCache(false) , hop(-1)
{
	//
}

//用来测试
DataPacket::DataPacket(ContentName name) : name(name) , packetType(DataPacketType) , 
						fromCache(false) , hop(-1)
{

}

DataPacket::DataPacket(ContentName name , unsigned size , bool from_cache) : 
	name(name) , size(size) , packetType(DataPacketType) , 
	fromCache(from_cache) , hop(0)
{
	//
}

DataPacket::DataPacket(string name , unsigned size , bool from_cache) : 
	name(ContentName(name)) , size(size) , 
	packetType(DataPacketType) , fromCache(from_cache) , hop(0)
{
	//
}