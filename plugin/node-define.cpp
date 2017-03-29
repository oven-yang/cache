#include<algorithm>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<cstring>
#include<vector>
#include<limits>
#include<cmath>
#include<utility>

using namespace std ;

int str_to_int(string str) ;

const int NAME_LENGTH = 2 ;//{1,2} 17576
static int OPERATION_KIND_NUM  ;//采用 Zipf 分布.(1+2+4+...

class Name
{
private :
	vector<vector<char> > name ;
public :
	Name(vector<int> length) ;
	bool isLast() ;
	Name next() ;
	string toString() ;
	Name randName() ;
	Name& operator++() ;
} ;

int main(int argc , char *argv[])
{
	if(argc != 3)
	{
		cout<<"parameters must be 3(program name,number of nodes,operation_kind_num)"<<endl ;
		return 0 ;
	}
	int node_num = str_to_int(argv[1]) ;
	if(node_num < 2)
	{
		cout<<node_num<<" node defined, node number should be greater than 1"<<endl ;
		return 0 ;
	}
	if((OPERATION_KIND_NUM = str_to_int(argv[2])) < 2)
	{
		cout<<pow(2 , OPERATION_KIND_NUM)-1<<" operation defined, too few, argument 3 should be greater than 1."<<endl ;
		return 0 ;
	}

	fstream node_file ;
	//先写./source/node-define/nodes文件,再写创建对应节点的定义文件node$i
	
	node_file.open("./source/node-define/nodes" , std::ios::out | std::ios::trunc) ;
	if(!node_file.is_open())
	{
		cout<<"file ./source/node-define/nodes not exists"<<endl ;
		return 0 ;
	}
	for(int i = 1 ; i <= node_num ; ++i)
	{
		node_file<<"node"<<i<<"\n" ;
	}
	node_file.close() ;

	for(int i = 1 ; i <= node_num ; ++i)//定义节点的属性,包含的内容,要进行的操作.
	{
		node_file.open("./source/node" + to_string(i) , std::ios::out | std::ios::trunc) ;
		node_file<<"[property]\n" ;
		node_file<<"interface_quantity 7\n" ;
		node_file<<"fib_capacity 1000\n" ;
		node_file<<"pit_capacity 1000\n" ;
		node_file<<"cs_capacity 500\n" ;
		node_file<<"pref_capacity 1000\n" ;
		node_file<<"popu_capacity 1000\n" ;
		node_file<<"capacity "<<numeric_limits<unsigned int>::max()<<"\n" ;
		node_file.close() ;
	}

	//define content
	for(int i = 1 ; i <= node_num ; ++i)
	{
		node_file.open("./source/node" + to_string(i) , std::ios::out | std::ios::app) ;
		node_file<<"[data]"<<endl ;//内容名需要保证不重复(file, media, audio, ......),且不同节点也不能存在相同的内容
		node_file.close() ;
	}
	Name name(vector<int>{1 , 2}) ;
	while(!name.isLast())
	{
		node_file.open("./source/node" + to_string(rand()%node_num + 1) , std::ios::out | std::ios::app) ;
		node_file<<name.toString()<<" "<<rand()%100 + 1<<endl ;
		node_file.close() ;
		++name ;
	}
	node_file.open("./source/node" + to_string(rand()%node_num + 1) , std::ios::out | std::ios::app) ;
	node_file<<name.toString()<<" "<<rand()%100 + 1<<endl ;
	node_file.close() ;

	//define operation
	for(int i = 1 ; i <= node_num ; ++i)
	{
		node_file.open("./source/node" + to_string(i) , std::ios::out | std::ios::app) ;
		node_file<<"[operation]\n" ;//timer content_name

		vector<pair<string , int> > opt ;
		for(size_t j = 0 ; j < OPERATION_KIND_NUM ; ++j)
		{
			string str_name("") ;
			while(str_name == "")
			{
				str_name = name.randName().toString() ;
				for(vector<pair<string , int> >::iterator it = opt.begin() ; it != opt.end() ; ++it)
				{
					if(str_name == it->first)
					{
						str_name = "" ;
						break ;
					}
				}
			}
			opt.push_back(make_pair(str_name , pow(2 , j))) ;
		}
		int j = 1 ;
		for( ; opt.size() > 0 ; ++j)
		{
			size_t k = rand()%opt.size() ;
			node_file<<j<<" "<<opt[k].first<<"\n" ;
			if(--opt[k].second == 0)
			{
				opt.erase(opt.begin()+k) ;
			}
		}
		node_file<<j<<" "<<name.randName().toString() ;
		
		node_file.close() ;
	}

	return 0 ;
}

int str_to_int(string str)
{
        std::istringstream stream(str);
        int num ;
        stream>>num ;
        return num ;
}

Name::Name(vector<int> length)
{
	for(vector<int>::iterator i = length.begin() ; i != length.end() ; ++i)
	{
		name.push_back(vector<char>(*i , 'a')) ;
	}
}

bool Name::isLast()
{
	for(vector<vector<char> >::iterator i = name.begin() ; i != name.end() ; ++i)
	{
		for(vector<char>::iterator j = i->begin() ; j != i->end() ; ++j)
		{
			if(*j < 'z')
			{
				return false ;
			}
		}
	}
	return true ;
}
Name Name::next()
{
	Name next_name = *this ;
	for(vector<vector<char> >::reverse_iterator i = next_name.name.rbegin() ; i != next_name.name.rend() ; ++i)
	{
		for(vector<char>::reverse_iterator j = i->rbegin() ; j != i->rend() ; ++j)
		{
			if(*j == 'z')
			{
				*j = 'a' ;
			}
			else
			{
				++*j ;
				return next_name ;
			}
		}
	}
	return *this ;
}
string Name::toString()
{
	string s("") ;
	for(vector<vector<char> >::iterator i = name.begin() ; i != name.end() ; ++i)
	{
		for(vector<char>::iterator j = i->begin() ; j != i->end() ; ++j)
		{
			s.push_back(*j) ;
		}
		s.push_back('/') ;
	}
	s.erase(--s.end()) ;
	return s ;
}
Name Name::randName()
{
	Name rand_name(*this) ;
	for(vector<vector<char> >::iterator i = name.begin() ; i != name.end() ; ++i)
	{
		for(vector<char>::iterator j = i->begin() ; j != i->end() ; ++j)
		{
			*j = static_cast<char>('a'+rand()%26) ;
		}
	}
	return rand_name ;
}
Name& Name::operator++()
{
	for(vector<vector<char> >::reverse_iterator i = name.rbegin() ; i != name.rend() ; ++i)
	{
		for(vector<char>::reverse_iterator j = i->rbegin() ; j != i->rend() ; ++j)
		{
			if(*j == 'z')
			{
				*j = 'a' ;
			}
			else
			{
				++*j ;
				return *this ;
			}
		}
	}
	return *this ;
}
