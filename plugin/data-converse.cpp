#include<iostream>
#include<fstream>
#include<string>

using namespace std ;

const int MAX = 20 , MIN = 10 , NODE_NUM = 20 ;

void process(string) ;

int main(void)
{
	process("avg-req-dist") ;
	process("cache-hit-rate") ;
	process("cache-replace-rate") ;
}

void process(string data_name)
{
	ifstream in ;
	ofstream out("./doc/data/" + data_name) ;
	out<<"op_num " ;
	for(int i = 1 ; i <= NODE_NUM ; ++i)
	{
		out<<"node"<<i<<" " ;
	}
	out<<endl ;
	for(int i = MIN ; i <= MAX ; ++i)
	{
		in.open("./doc/data/" + to_string(i) + "/" + data_name) ;
		string node ;
		double value ;
		out<<i<<" " ;
		int cnt = 1 ;
		while(in>>node>>value)
		{
			if(node != "node" + to_string(cnt++))
			{
				cout<<data_name<<"line "<<i-1<<endl ;
				return ;
			}
			out<<value<<" " ;
		}
		out<<endl ;
		in.close() ;
	}
	out.close() ;
}
