/*
 * @ovenyang
 * 前期此文件用来对各部分进行测试,目前此程序就只有这个作用,程序其他任何部分对此文件没有任何依赖.
 * 待程序其他部分完成,此文件为程序的主体,各部分皆由此文件控制.
 * 其任务有
 * 		1.初始化程序
 * 		2.处理输入输出
 * 		3.交互
 */
#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<list>
#include<vector>
#include<sstream>
#include"../header/ContentName.h"
#include"../header/ContentStore.h"
#include"../header/InterestPacket.h"
#include"../header/DataPacket.h"
#include"../header/Interface.h"
#include"../header/PendingInterestTable.h"
#include"../header/ForwardingInterestBase.h"
#include"../header/Node.h"

using namespace std ;

const string NODES_DEF_FILE("./source/node-define/nodes") ;
const string LINK_FILE("./source/link") ;
const int INFINITY = 1000 ;//INFINITY表示节点间的距离为无穷大.

static int NODE_NUM ;
static set<string> nodes ;
static vector<Node> node_list ;
//为简单起见，规定node_list[i] 节点对应node_links中的 i 节点,node_links[i][j] == 1表示节点i与j相连。特此规定!!!!!!
static vector<vector<int> > node_links ;

//读取../source/node-define目录下的文件，初始化各个节点的属性，
//包括get_task_queue.
//读取../source/link文件，初始化各个节点的连接（即接口状态）
void init() ;
string read_property(ifstream& , map<string , unsigned>&) ;
string read_data(ifstream& , Node*) ;
string read_operation(ifstream& , Node*) ;
void init_log(vector<Node>&) ;

void print() ;

int main()
{
// 	cout<<ContentName("a/ai").getStrName()<<endl ;
	
	init() ;
	bool has_work = true ;
	while(has_work)
	{
		++global_clock ;
		has_work = false ;
		for(vector<Node>::iterator it = node_list.begin() ; it != node_list.end() ; ++it)
		{
			if(it->hasWork())
			{
				has_work = true ;
				it->work() ;
			}
		}
	}
	log("end at " + int_to_str(global_clock)) ;
	cout<<"------------------program stopped---------------"<<endl ;
	cout<<"------------------program stopped---------------"<<endl ;
	cout<<"------------------program stopped---------------"<<endl ;
	cout<<"------------------program stopped---------------"<<endl ;
	cout<<"------------------program stopped---------------"<<endl ;
	return 0 ;
}

void init()
{
	log("init start") ;
	global_clock = 0 ;

	//初始化各个节点的属性

	//读入节点列表文件 nodes
	ifstream in(NODES_DEF_FILE) ;
	if(!in.is_open())
	{
		return ;
	}
	for(string str ; in>>str ; )
	{
		nodes.insert(str) ;
	}
	in.close() ;
	NODE_NUM = nodes.size() ;
	node_links = vector<vector<int> >(NODE_NUM , vector<int>(NODE_NUM , INFINITY)) ;

	ifstream node_file ;
	//cap_list保存各个数据结构的容量
	map<string , unsigned> cap_list ;
	for(set<string>::const_iterator it = nodes.begin() ; it != nodes.end() ; ++it)
	{
		//*it为节点名字
		// cap_list.clear() ;
/////////////////////////////////////////////////////////////////////////////////
		node_file.open("./source/" + *it) ;
		if(!node_file.is_open())
		{
			throw runtime_error("file open error:init():open " + (*it) + " failed\n") ;
		}
/////////////////////////////////////////////////////////////////////////////////
		string read("") ;
		node_file>>read ;
		if(read != "[property]")
		{
			cout<<"ERROR:"<<*it<<" define file cannot find [property] tag"<<endl ;
			exit(1) ;
		}
		if(read_property(node_file , cap_list) != "[data]")
		{
			cout<<"ERROR:"<<*it<<" define file cannot find [data] tag"<<endl ;
			exit(1) ;
		}
		// Node *node = new Node(*it , cap_list) ;
		node_list.push_back(Node(*it , cap_list)) ;
		if(read_data(node_file , &node_list[node_list.size()-1]) != "[operation]")
		{
			cout<<"ERROR:"<<*it<<" define file cannot find [operation] tag"<<endl ;
			exit(1) ;
		}
		read_operation(node_file , &node_list[node_list.size()-1]) ;
		cout<<"node "<<(*it)<<" initialized, "<<node_list.rbegin()->data_table.size()<<" content, "<<node_list.rbegin()->get_task_queue.size()<<" task"<<endl ;
		node_file.close() ;
	}//for(set<string>::iterator it = nodes.begin() ; it != nodes.end() ; ++it)
	//节点基本属性初始化完成

	for(vector<Node>::iterator it = node_list.begin() ; it != node_list.end() ; ++it)
	{
		it->init() ;
	}

	//根据 ./source/link 文件初始化节点的连接状况.
	ifstream link_file(LINK_FILE) ;
	if(!link_file.is_open())
	{
		throw runtime_error("ERROR:file " + LINK_FILE + " not exists or occur some errors\n") ;
	}
	
	unsigned line = 1 ;
	string node_a , node_b ;
	for( ; link_file>>node_a>>node_b && !node_a.empty() && !node_b.empty() ; ++line)
	{
		size_t it_a(node_list.size()) , it_b(node_list.size()) ;
		for(size_t it(0) ; it != node_list.size() ; ++it)
		{
			if(node_list[it].name == node_a)
			{
				it_a = it ;
			}
			else if(node_list[it].name == node_b)
			{
				it_b = it ;
			}
		}
		if(it_a == node_list.size() || it_b == node_list.size())
		{
			throw runtime_error("ERROR:file " + LINK_FILE + ", line " + int_to_str(line) + ":cannot find the  node\n") ;
		}
		if(!node_list[it_a].buildLink(&node_list[it_b]))//建立连接，检查连接是否失败
		{
			node_list[it_a].closeLink(&node_list[it_b]) ;
			throw runtime_error("ERROR:file " + LINK_FILE + ", line " + int_to_str(line) + ":link failed\n") ;
		}
		if(!node_list[it_b].buildLink(&node_list[it_a]))//建立连接，检查连接是否失败
		{
			node_list[it_b].closeLink(&node_list[it_a]) ;
			throw runtime_error("ERROR:file " + LINK_FILE + ", line " + int_to_str(line) + ":link failed\n") ;
		}
		if(node_links[it_a][it_b] == 1 || node_links[it_a][it_b] == 1)
		{
			throw runtime_error("ERROR:file " + LINK_FILE + ", line " + int_to_str(line) + ":the link exists in node_links\n") ;
		}
 		node_links[it_a][it_b] = node_links[it_b][it_a] = 1 ;
		cout<<"read link: "<<node_a<<"<--->"<<node_b<<"."<<endl ;
	}//for(string str ; getline(link_file , str) ; ++line)
	for(size_t it(0) ; it != node_links.size() ; ++it)
	{
		node_links[it][it] = 0 ;
	}
	link_file.close() ;


	//初始化各个节点的路由表,假设INFINITY表示节点间的距离为无穷大.利用Floyd算法
	{
		typedef vector<vector<int> > W ;
		//path[i][j]表示节点node_list[i]到节点node_list[j]的下一跳为node_list[patj[i][j]]
		W path(NODE_NUM , vector<int>(NODE_NUM , NODE_NUM)) ;
		W *dist = new W(node_links) ;
		for(size_t i = 0 ; i < NODE_NUM ; ++i)
		{
			for(size_t j = 0 ; j < NODE_NUM ; ++j)
			{
				path[i][j] = j ;
			}
		}
		for(size_t k = 0 ; k < NODE_NUM ; ++k)
		{
			for(size_t i = 0 ; i < NODE_NUM ; ++i)
			{
				for(size_t j = 0 ; j < NODE_NUM ; ++j)
				{
					if((*dist)[i][k] + (*dist)[k][j] < (*dist)[i][j])
					{
						(*dist)[i][j] = (*dist)[i][k] + (*dist)[k][j] ;
						path[i][j] = path[i][k] ;
					}
				}
			}
		}
		for(size_t i = 0 ; i < NODE_NUM ; ++i)//初始化节点i的路由表
		{
			for(size_t j = 0 ; j < NODE_NUM ; ++j)
			{
				if(i == j)
				{
					continue ;
				}
				if((*dist)[i][j] >= INFINITY)
				{
					throw runtime_error(node_list[i].getName() + " to " + node_list[j].getName() +
					" is infinity.\n") ;
				}
				if(path[i][j] >= NODE_NUM)
				{
					throw runtime_error("path from " + node_list[i].getName() + " to " + node_list[j].getName() +
					" is illegal.\n") ;
				}
				if(node_list[i].isLinked(&node_list[path[i][j]]) == false)
				{
					throw runtime_error(node_list[i].getName() + " to " + node_list[j].getName() + 
					" through " + node_list[path[i][j]].getName() + " while " + node_list[i].getName() + 
					" to " + node_list[path[i][j]].getName() + " is infinity.\n") ;
				}
				// Interface *interface = node_list[i].getLinkInterface(&node_list[j]) ;
				Interface *interface ;
// 				size_t k = j ;
// 				while(!node_list[i].isLinked(&node_list[k]))
// 				{
// 					k = path[i][k] ;
// 				}
				interface = node_list[i].getLinkInterface(&node_list[path[i][j]]) ;
				for(map<ContentName,unsigned>::iterator it = node_list[j].data_table.begin() ; it != node_list[j].data_table.end() ; ++it)
				{
					node_list[i].routing_table.add(it->first , interface) ;
				}
			}
		}
		
	}

	init_log(node_list) ;

	log("init finished") ;
}

string read_property(ifstream& file , map<string , unsigned>& capacity)
{
	string str ;
	unsigned val ;
	while(file>>str && str != "[data]" && file>>val)
	{
		capacity[str] = val ;
	}
	return str ;
}

string read_data(ifstream& file , Node* node)
{
	string str ;
	unsigned val ;
	while(file>>str && str != "[operation]" && file>>val)
	{
		if(!node->addContent(str , val))
		{
			log(node->getName() + "add content " + str + "(" + int_to_str(val) + ") faild") ;
		}
	}
	return str ;
}

string read_operation(ifstream& file , Node* node)
{
	unsigned val ;
	string str ;
	while(file>>val>>str)
	{
		if(!node->initATask(val , str))
		{
			log(node->getName() + "init a task " + str + "(" + int_to_str(val) + ") faild") ;
		}
	}
	return "" ;
}

void init_log(vector<Node>& node)
{
	for(vector<Node>::iterator it = node.begin() ; it != node.end() ; ++it)
	{
		log(it->getName() , "CSCapacity " + int_to_str(it->getCSCapacity())) ;
	}
}

void print()
{
	for(vector<Node>::iterator it = node_list.begin() ; it != node_list.end() ; ++it)
	{
		cout<<it->name<<" "<<it->get_task_record.size()<<endl ;
	}
}
