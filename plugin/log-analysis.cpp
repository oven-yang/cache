#include<iostream>
#include<fstream>
#include<string>

using namespace std ;

static int NODE_NUM ;

static string request_kind ;

bool write(double , string) ;

int main(int argc , char* argv[])// ./log-analysis request-kind topology(CERNET2/Deltacom/GtsCe/Oteglobe) cache-strategy(PPDS/LRU/no-cache)
{

	if(argc != 4)
	{
		cout<<"log-analysis:para error"<<endl ;
		return 0 ;
	}
	request_kind = argv[1] ;

	string topology = argv[2] ;
	if(topology == "CERNET2")
	{
		NODE_NUM = 20 ;
	}
	else if(topology == "Deltacom")
	{
		NODE_NUM = 113 ;
	}
	else if(topology == "GtsCe")
	{
		NODE_NUM = 149 ;
	}
	else if(topology == "Oteglobe")
	{
		NODE_NUM = 93 ;
	}
	else
	{
		cout<<"analysis:para 3(topology) wrong. only CERNET2/Deltacom/GtsCe/Oteglobe is supported"<<endl ;
		return 0 ;
	}

	string cache_strategy = argv[3] ;
	if(cache_strategy != "PPDS" && cache_strategy != "LRU" && cache_strategy != "no-cache")
	{
		cout<<"log-analysis:para 4(cache strategy) wrong. only PPDS/LRU/no-cache is supported"<<endl ;
		return 0 ;
	}

	ifstream file ;
	int req_num = 0 ;
	int hit_num = 0 , rep_num = 0 , cache_num = 0 , hop_sum = 0 ;
	for(int i = 1 ; i <= NODE_NUM ; ++i)
	{
		file.open("./source/log/node" + to_string(i)) ;
		if(!file.is_open())
		{
			cout<<"./source/log/node"<<i<<" open failed in log-analysis."<<endl ;
			return 0 ;
		}
		
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
				//...
			}
		}

		file.close() ;
	}
	write(1.0 * hit_num / req_num , "./doc/data/" + topology + ".topology/" + cache_strategy  + "/cache-hit-rate") ;
	write(1.0 * rep_num / cache_num , "./doc/data/" + topology + ".topology/" + cache_strategy  + "/cache-replace-rate") ;
	write(1.0 * hop_sum / req_num , "./doc/data/" + topology + ".topology/" + cache_strategy  + "/avg-req-dist") ;	
	return 0 ;
}

bool write(double val , string file)
{
	ofstream record(file , ios::out | ios::app) ;
	if(!record.is_open())
	{
		cout<<file<<" open failed."<<endl ;
		return 0 ;
	}
	record<<request_kind<<" "<<val<<endl ;
	record.close() ;
}

/*
bool write(int node , double val , string file) ;

int main()
{
	ifstream file ;
	for(int i = 1 ; i <= NODE_NUM ; ++i)
	{
		file.open("./source/log/node" + to_string(i)) ;
		if(!file.is_open())
		{
			cout<<"./source/log/node"<<i<<" open failed."<<endl ;
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
*/