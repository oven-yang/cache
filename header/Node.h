#ifndef NODE_H
#define NODE_H

#include"Interface.h"
#include"global.h"
#include"DataPacket.h"
#include"InterestPacket.h"
#include"ContentName.h"
#include"ContentStore.h"
#include"PendingInterestTable.h"
#include"PopularityTable.h"
#include"PreferenceTable.h"
#include"RoutingTable.h"
#include<algorithm>
#include<vector>
#include<string>
#include<utility>
#include<map>
#include<set>
#include<queue>
#include<list>

using std::string ;
using std::map ;
using std::list ;
using std::vector ;
using std::set ;
using std::queue ;

enum TaskType{SendData , SendInterest , ReceiveData , ReceiveInterest} ;

class Node
{
private :
	friend void init() ;
	friend void print() ;

	// struct compGetTask
	// {
	// 	bool operator ()(pair<timer , ContentName> &task1 , pair<timer , ContentName> &task2)
	// 	{
	// 		return task1.first < task2.first || 
	// 			(task1.first == task2.first && task1.second < task2.second) ;
	// 	}
	// } ;

	string name ;//name唯一表示一个node.

	// 注意：在通过复制构造函数创建的Node类中， interface_list中接口0对应的节点并不是节点本身，而是复制构造函数参数节点。
	vector<pair<Interface , Node*> > interface_list ;//接口列表,<接口,接口所连节点/nullptr>

	//节点本身存储的内容.
	map<ContentName , unsigned> data_table ;//need initialize
	//list<map<ContentName , unsigned>::iterator> data_table_size_priority ;
	unsigned data_size ;

	//初始化时将节点的operation保存在此容器中
	// priority_queue<pair<timer , ContentName> , vector<pair<timer , ContentName> > , compGetTask> get_task_queue ;//need initialize
	queue<pair<unsigned , ContentName> > get_task_queue ;//need initialize
	//get_task_record记录本节点get_task对内容content已经发出的未被满足的请求的数量
	//初始为空，当发出请求之后将,将请求的内容名对应值加一
	//当节点要发出一个内容时，检查是否要向0接口发出，若是，则将get_task_record中对应内容名删除，
	//并答应对应若干条get task已满足的日志
	map<ContentName , int> get_task_record ;

	RoutingTable routing_table ;//need initialize

	unsigned capacity ;

	PendingInterestTable pit ;
	ContentStore cs ;

	list<ContentName> cs_abstract ;

	map<ContentType , double> basic_cache_priority_table ;
	PopularityTable popularity_table ;
	PreferenceTable preference_table ;
	
	queue<TaskType> task_queue ;
    queue<pair<DataPacket , set<Interface> > > send_data_queue ;
    queue<pair<InterestPacket , Interface> > send_interest_queue ;
    queue<pair<DataPacket , Node*> > receive_data_queue ;
    queue<pair<InterestPacket , Node*> > receive_interest_queue ;

	double getBasicCachePriority(ContentName name)
	{
		return basic_cache_priority_table.count(name.getContentType()) == 1 ? basic_cache_priority_table[name.getContentType()] : 0 ;
	}

	unsigned getDegree() ;

public :
	// Node(string name) : name(name) {}
	// Node(string name , unsigned interface_quantity) ;
	// Node(string name , unsigned interface_quantity , unsigned capacity , unsigned fib_capacity , unsigned pit_capacity , unsigned cs_capacity , unsigned pref_capacity , unsigned popu_capacity , map<ContentName , unsigned> data) ;
	Node(const string name , map<string , unsigned> capacity_list) ;

	void init() ;

	bool setPITCapacity(unsigned new_capacirt){ return pit.setCapacity(new_capacirt) ; }
	bool setPrefCapacity(unsigned new_capacity){ return preference_table.setCapacity(new_capacity) ; }
	bool setPopuCapacity(unsigned new_capacity){ return popularity_table.setCapacity(new_capacity) ; }
	void rename(string new_name){ name = new_name ; }
	string getName() const { return name ; }
	unsigned getPITCapacity(){ return pit.getCapacity() ; }
	unsigned getCSCapacity(){ return cs.getCapacity() ; }
	unsigned getPITDataSize(){ return pit.getItemQuantity() ; }
	unsigned getCSDataSize(){ return cs.getDataSize() ; }
	unsigned getGetTaskNum(){ return get_task_queue.size() ; }
	InterfaceState getInterfaceState(Interface interface) ;
	// set<Node*> getNeighbour() ; //illegal
	bool isLinked(Node* neighbour) ;
	bool hasWork() ;
	bool addContent(string , unsigned) ;
	bool initATask(unsigned , string) ;

	// 返回与neighbour相连的接口,调用前最好确保已经与此节点相连,即isLinked() == true.
	// 否则返回0接口表示没有和此节点相连
	// Interface getLinkInterface(Node *neighbour) ;
	Interface* getLinkInterface(Node *neighbour) ;
	
	bool buildLink(Node *neighbour) ;
	void closeLink(Node *neighbour) ;

	void addTask(DataPacket data_packet , set<Interface> interfaces) ;
    void addTask(InterestPacket interest_packet , Interface interface) ;
    void addTask(DataPacket data_packet , Node* neighbour) ;
    void addTask(InterestPacket interest_packet , Node* neighbour) ;
    void executeTask() ;

	bool sendPacket(DataPacket data_packet , set<Interface> interfaces) ;
	bool sendPacket(InterestPacket interest_packet , Interface interface) ;
	bool receivePacket(DataPacket data_packet ,  Node* sender) ;
	bool receivePacket(InterestPacket interest_packet , Node* sender) ;

	Interface* getForwardInterface(ContentName name) ;

	void updateCachePriority() ;

	void cache(DataPacket data_packet) ;

	void work() ;

	inline bool operator==(const Node& node1)
	{
		return name == node1.getName() ;
	}
	inline bool operator!=(const Node& node1)
	{
		return name != node1.getName() ;
	}
} ;

#endif
