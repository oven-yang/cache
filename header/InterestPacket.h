#ifndef INTEREST_PACKET_H
#define INTEREST_PACKET_H

#include"ContentName.h"
#include"global.h"
#include<string>

using namespace std ;

class InterestPacket
{
private :
	ContentName name ;
	PacketType packetType ;
public :
	InterestPacket(string name) ;
	InterestPacket(ContentName name) ;
	ContentName getName(){ return name ; }
	PacketType getPacketType(){ return packetType ; }
	void setName(ContentName new_name){ name = new_name ; }
	void setName(string new_name) ;
} ;

#endif
