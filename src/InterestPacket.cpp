#include"../header/InterestPacket.h"

InterestPacket::InterestPacket(string name) : name(ContentName(name)) , packetType(InterestPacketType)
{
	//
}

InterestPacket::InterestPacket(ContentName name) : name(name) , packetType(InterestPacketType)
{
	//
}

void InterestPacket::setName(string new_name)
{
	name = ContentName(new_name) ;
}
