#include"../header/Node.h"
#include<stdexcept>


// 默认0接口关闭，需要手动开启，并使其指向自身。
Node::Node(const string name , map<string , unsigned> capacity_list) : 
	name(name) , capacity(capacity_list["capacity"]) , 
	pit(capacity_list["pit_capacity"]) , fib(capacity_list["fib_capacity"]) , 
	cs(capacity_list["cs_capacity"]) , data_size(0) , 
	popularity_table(capacity_list["popu_capacity"]) , 
	preference_table(capacity_list["pref_capacity"])
{
	for(unsigned i = 0 ; i <= capacity_list["interface_quantity"] ; ++i)//初始化接口列表
	{
		interface_list.push_back(make_pair(Interface(i) , nullptr)) ;
		// interface_list.rbegin()->second = 0 ;
	}
}

void Node::init()
{
	interface_list[0].second = this ;
	interface_list[0].first.setState(linked) ;
}

InterfaceState Node::getInterfaceState(Interface interface)//当此接口不存在时返回closed
{
	for(vector<pair<Interface , Node*> >::iterator it = ++interface_list.begin() ; it != interface_list.end() ; ++it)
	{
		if(it->first == interface)
		{
			return it->first.getState() ;
		}
	}
	return closed ;
}

bool Node::isLinked(Node* neighbour)
{
	if(neighbour == 0 && neighbour->name == "" && this->name == "")
	{
		throw runtime_error(name + " called isLinked(0)\n") ;
	}
	for(vector<pair<Interface , Node*> >::iterator it = interface_list.begin() ; it != interface_list.end() ; ++it)
	{
		if(it->second->name == neighbour->name 
			&&
			it->first.getState() >= linked)
		{
			return true ;
		}
	}
	return false ;
}

bool Node::hasWork()
{
	return !(get_task_queue.empty() && task_queue.empty()) ;
}

Interface* Node::getLinkInterface(Node *neighbour)
{
	for(vector<pair<Interface , Node*> >::iterator it = ++interface_list.begin() ; it != interface_list.end() ; ++it)
	{
		if(it->second->getName() == neighbour->getName() && it->first.getState() >= linked)
		{
			return &(it->first) ;
		}
	}
	return &(interface_list.begin()->first) ;//表示未与节点neighbour相连.
}

bool Node::buildLink(Node *neighbour)//建立后即可接受该节点的包,向该节点发送包(但是可能该节点拒绝接收)
{
	for(vector<pair<Interface , Node*> >::iterator it = interface_list.begin() ; it != interface_list.end() ; ++it)
	{
		if(it->first.getId() == 0) continue ;
		if(it->first.getState() <= open)//只要有空闲接口就使用,不考虑接口是否打开.
		{
			it->first.setState(linked) ;
			it->second = neighbour ;
			return true ;
		}
	}
	return false ;
}

void Node::closeLink(Node *neighbour)//同上buildLink()
{
	for(vector<pair<Interface , Node*> >::iterator it = interface_list.begin() ; it != interface_list.end() ; ++it)
	{
		if(it->first.getId() == 0) continue ;
		if(it->second->getName() == neighbour->getName())
		{
			it->first.setState(open) ;
			it->second = 0 ;
			return ;
		}
	}
}

//将发送data packet的任务入队
void Node::addTask(DataPacket data_packet)
{
    task_queue.push(SendData) ;
    send_data_queue.push(data_packet) ;
	// log(name , "node " + this->name + " add send_data task " + data_packet.getName().getStrName() + "->" + int_to_str(interface.getId())) ;
}

//将发送interest packet的任务入队
void Node::addTask(InterestPacket interest_packet)
{
	// log(name , "node " + this->name + " is going to add send_interest task " + interest_packet.getName().getStrName()) ;
    task_queue.push(SendInterest) ;
    send_interest_queue.push(interest_packet) ;
	// log(name , "node " + this->name + " add send_interest task " + interest_packet.getName().getStrName()) ;
}

//将接收data packet的任务入队，由邻接点调用此函数，相当于向此节点发送packet
void Node::addTask(DataPacket data_packet , Node* neighbour)
{
	// log(name , "node " + this->name + " is going to add receive_data task " + data_packet.getName().getStrName() + "<-" + neighbour->getName()) ;
    if(isLinked(neighbour) == false)
	{
		//未与neighbor节点相连,不处理该packet
		return ;
	}
    task_queue.push(ReceiveData) ;
    receive_data_queue.push(make_pair(data_packet , neighbour)) ;
	// log(name , "node " + this->name + " add receive_data task " + data_packet.getName().getStrName() + "<-" + neighbour->getName()) ;
}

//将接收interest packet的任务入队，由邻接点调用此函数，相当于向此节点发送packet
void Node::addTask(InterestPacket interest_packet , Node* neighbour)
{
	// log(name , "node " + this->name + " is going to add receive_interest task " + interest_packet.getName().getStrName() + "<-" + neighbour->getName()) ;
	if(isLinked(neighbour) == false)
	{
		//未与neighbor节点相连,不处理该packet
		return ;
	}
	task_queue.push(ReceiveInterest) ;
	receive_interest_queue.push(make_pair(interest_packet , neighbour)) ;
	// log(name , "node " + this->name + " add receive_interest task " + interest_packet.getName().getStrName() + "<-" + neighbour->getName()) ;
}

void Node::executeTask()
{
	if(task_queue.empty())
	{
		return ;
	}
	switch(task_queue.front())
	{
		case SendData :
		{
			set<Interface> interfaces ;
			if(pit.isExist(send_data_queue.front().getName()))
			{
				interfaces = pit.getInterfaces(send_data_queue.front().getName()) ;
				pit.remove(send_data_queue.front().getName()) ;
			}
			sendPacket(send_data_queue.front() , interfaces) ;
			send_data_queue.pop() ;
			break ;
		}
		case SendInterest :
		{
			ContentName name = send_interest_queue.front().getName() ; 
			if(fib.isExist(name))
			{
				sendPacket(send_interest_queue.front() , fib.getInterfaces(name)) ;
				fib.remove(name) ;
			}
			send_interest_queue.pop() ;
			break ;
		}
		case ReceiveData :
		{
			receivePacket(receive_data_queue.front().first , receive_data_queue.front().second) ;
			receive_data_queue.pop() ;
			break ;
		}
		case ReceiveInterest :
		{
			receivePacket(receive_interest_queue.front().first , receive_interest_queue.front().second) ;
			receive_interest_queue.pop() ;
			break ;
		}
		// default :
	}
	task_queue.pop() ;
}

bool Node::sendPacket(DataPacket data_packet , set<Interface> interfaces)
{
	data_packet.riseHop() ;
	bool con = true ;
	for(set<Interface>::iterator interface = interfaces.begin() ; interface != interfaces.end() ; ++interface)
	{
		if(interface->getId() == 0)
		{
			map<ContentName , int>::iterator it = get_task_record.find(data_packet.getName()) ;
			if(it == get_task_record.end())
			{
				log(this->name , "error:get task error " + data_packet.getName().getStrName()) ;
			}
			else
			{
				while((it->second)-- > 0)
				{
					log(this->name , "get " + data_packet.getName().getStrName() + " " + int_to_str(data_packet.getSize()) + " " + 
					data_packet.isFromCache() + " " + int_to_str(data_packet.getHop())) ;
				}
				get_task_record.erase(it) ;
			}
		}
		else
		{
			for(vector<pair<Interface , Node*> >::iterator it = interface_list.begin() ; it != interface_list.end() ; ++it)
			{
				if(it->first == *interface && it->first.getState() == linked && it->second != this)
				{
					it->second->addTask(data_packet , this) ;
					break ;
				}
				else
				{
					con = false ;
				}
			}
		}
	}
	// log(name , "node " + this->name + " finish send content " + data_packet.getName().getStrName()) ;
	//接口错误或者没有建立连接.
	return con ;
}

bool Node::sendPacket(InterestPacket interest_packet , set<Interface> interfaces)
{
	bool con = true ;
	for(set<Interface>::iterator interface = interfaces.begin() ; interface != interfaces.end() ; ++interface)
	{
		for(vector<pair<Interface , Node*> >::iterator it = interface_list.begin() ; it != interface_list.end() ; ++it)
		{
			if(it->first == *interface && it->first.getState() == linked && it->second != this)
			{
				it->second->addTask(interest_packet , this) ;
				break ;
			}
			else
			{
				con = false ;
			}
		}
	}
	return con ;
}

bool Node::receivePacket(DataPacket data_packet , Node* sender)
{
	bool flag = false ;
	ContentName packet_name = data_packet.getName() ;
	Interface interface(0) ;
	for(vector<pair<Interface , Node*> >::iterator it = interface_list.begin() ;
	it != interface_list.end() ; ++it)
	{
		if(*sender == *(it->second) && it->first.getState() == linked)
		{
			interface = it->first ;
			flag = true ;
			break ;
		}
	}
	if(flag == false)
	{
		return false ;
	}
	if(pit.isExist(packet_name))
	{
		addTask(data_packet) ;
	}
	if(data_table.count(packet_name) == 0)
	{
		cache(data_packet) ;
	}
	return true ;
}

bool Node::receivePacket(InterestPacket interest_packet , Node* sender)
{
	bool flag = false ;
	Interface interface ;
	for(vector<pair<Interface , Node*> >::iterator it = interface_list.begin() ; it != interface_list.end() ; ++it)
	{
		if(*sender == *(it->second) && it->first.getState() == linked)
		{
			interface = it->first ;
			flag = true ;
			break ;
		}
	}
	if(flag == false)
	{
		return false ;
	}
	if(interface.getId() == 0)
	{
		preference_table.updateQuantity(interest_packet.getName()) ;
	}
	else
	{
		popularity_table.updateQuantity(interest_packet.getName()) ;
	}

	if(data_table.count(interest_packet.getName()))//节点本身存储了此内容
	{
		pit.add(interest_packet.getName() , interface) ;
		addTask(DataPacket(interest_packet.getName() , data_table[interest_packet.getName()] , false)) ;
		return true ;
	}
	else if(cs.isExist(interest_packet.getName()))//CS中缓存了此内容
	{
		pit.add(interest_packet.getName() , interface) ;
		addTask(cs.getDataPacket(interest_packet.getName())) ;
		return true ;
	}
	else if(!pit.isExist(interest_packet.getName()))//PIT中不存在此请求
	{
		pit.add(interest_packet.getName() , interface) ;
		if(!fib.isExist(interest_packet.getName()))
		{
			addTask(interest_packet) ;
			Interface *forward_interface = getForwardInterface(interest_packet.getName()) ;
			if(forward_interface == 0)//无法确定此interest packet应该向哪个接口转发。
			{
				// throw runtime_error("routing error:node " + name + " receivePacket():get " + interest_packet.getName().getStrName()) ;
				log(name , "routing error:cannot forword \"" + interest_packet.getName().getStrName() + "\"") ;
				return false ;
			}
			fib.add(interest_packet.getName() , *forward_interface) ;
		}
	}
	return true ;
}

void Node::work()//执行一次此函数耗费一个单位时间
{
	// log(name , "node " + this->name + " start work at " + int_to_str(global_clock)) ;
	if(global_clock % update_circle == 0)
	{
		updateCachePriority() ;
	}
	while(!get_task_queue.empty() && global_clock >= get_task_queue.front().first)
	{
		addTask(InterestPacket(get_task_queue.front().second) , this) ;

		//下面的语句依赖于map<ContentName , int>将value值默认设为0，
		//当这点我并不确定，可能出现问题，用它下面的那两条语句更加保险。
		//++get_task_record[get_task_queue.front().second] ;
		map<ContentName , int>::iterator it = get_task_record.find(get_task_queue.front().second) ;
		it == get_task_record.end() ? (get_task_record[get_task_queue.front().second] = 1) : (++(it->second)) ;
		get_task_queue.pop() ;
	}
	this->executeTask() ;
}

Interface* Node::getForwardInterface(ContentName name)
{
	return routing_table.getInterface(name) ;
}

void Node::cache(DataPacket data)
{
	/*
	 * 当没有记录d此packet的基本缓存优先级时,设此优先级为0 
	 */
	double cache_priority = calculateCachePriority(getBasicCachePriority(data.getName()) , data.getSize()) ;
	unsigned old_cs_data_size = cs.getDataSize() ;
	string con = cs.add(data , cache_priority) ;
	if(con == "replace")
	{
		// 记录 replace remove_data_size inserted_data_name nserted_data_size 
		log(name , "replace " + int_to_str(old_cs_data_size - cs.getDataSize() + data.getSize()) + " " + data.getName().getStrName() + " " + int_to_str(data.getSize())) ;
	}
	else if(con == "cached")
	{
		log(name , "store " + data.getName().getStrName() + " " + int_to_str(data.getSize())) ;
	}
}

//基于流行度,偏好度,更新basic_cache_priority_table和cs中的cache_priority
void Node::updateCachePriority()
{
	for(map<ContentName , double>::iterator it = basic_cache_priority_table.begin() ; it != basic_cache_priority_table.end() ; ++it)
	{
		it->second *= old_weight ;
	}
	for(PopularityTable::iterator it = popularity_table.begin() ; it != popularity_table.end() ; ++it)
	{
		basic_cache_priority_table[it->first] += (1-old_weight)*(popularity_table.getRate(it->first)*popu_weight + preference_table.getRate(it->first)*pref_weight) ;
	}
	for(list<pair<DataPacket , double> >::iterator it = cs.content.begin() ; it != cs.content.end() ; ++it)
	{
		it->second = calculateCachePriority(getBasicCachePriority(it->first.getName()) , it->first.getSize()) ;
	}
	popularity_table.clear() ;
	preference_table.clear() ;
}

unsigned Node::getDegree()
{
	vector<pair<Interface , Node*> >::iterator it = interface_list.begin() ;
	unsigned cnt = 0 ;
	for(++it ; it != interface_list.end() ; ++it)
	{
		if(it->first.getState() >= linked)
		{
			++cnt ;
		}
	}
	return cnt ;
}

bool Node::addContent(string name , unsigned size)
{
	if(data_size + size > capacity)
	{
		return false ;
	}
	data_table[ContentName(name)] = size ;
	data_size += size ;
	return true ;
}

bool Node::initATask(unsigned time , string name)
{
	if(time < 1)
	{
		return false ;
	}
	get_task_queue.push(make_pair(time , ContentName(name))) ;
	return true ;
}
