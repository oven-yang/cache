/*
 * 此文件用来进行测试,程序任何部分都对此文件没有任何依赖,程序完成时可以此文件删除.
 */
#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<list>
#include<vector>
#include<sstream>
#include"./header/ContentName.h"
#include"./header/ContentStore.h"
#include"./header/InterestPacket.h"
#include"./header/DataPacket.h"
#include"./header/Interface.h"
#include"./header/PendingInterestTable.h"
#include"./header/ForwardingInterestBase.h"
#include"./header/Node.h"

using namespace std ;

int main()
{
	Interface i0(0) , i1(1) ;
	RoutingTable a ;
	a.add(ContentName("a/bc") , &i0) ;
	a.add(ContentName("a/bb") , &i0) ;
    return 0 ;
}
