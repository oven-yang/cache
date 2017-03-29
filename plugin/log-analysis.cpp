#include<iostream>
#include<fstream>
#include<string>

using namespace std ;

const int NODE_NUM = 20 ;

bool write(int node , double val , string file) ;

int main()
{
	ifstream file ;
	for(int i = 1 ; i <= NODE_NUM ; ++i)
	{
		file.open("./log-backup/node" + to_string(i)) ;
		if(!file.is_open())
		{
			cout<<"./log-backup/node"<<i<<" open failed."<<endl ;
			return 1 ;
		}
		int req_num = 0 ;
		int hit_num = 0 , rep_num = 0 , cache_num = 0 , hop_sum = 0 ;
		string str ;
		while(file>>str && str != "")
		{
			if(str == "get")
			{
				++req_num ;
				file>>str>>str>>str ;
				if(str == "cache")
				{
					++hit_num ;
				}
				int hop ;
				file>>hop ;
				hop_sum += hop ;
				file>>str ;
			}
			else if(str == "replace")
			{
				++rep_num ;
				++cache_num ;
			}
			else if(str == "store")
			{
				++cache_num ;
			}
			else
			{
				
			}
		}
		write(i , 1.0 * hit_num / req_num , "./conclusion/cache-hit-rate") ;
		write(i , 1.0 * rep_num / cache_num , "./conclusion/cache-replace-rate") ;
		write(i , hop_sum / req_num , "./conclusion/avg-req-dist") ;
		file.close() ;
	}
	return 0 ;
}

bool write(int node , double val , string file)
{
	ofstream record(file , ios::app) ;
	if(!record.is_open())
	{
		cout<<file<<" open failed."<<endl ;
		return 1 ;
	}
	record<<"node"<<node<<" "<<val<<"\n" ;
	record.close() ;
}
