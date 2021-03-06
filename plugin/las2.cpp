#include<iostream>
#include<fstream>
#include<string>
#include<limits>

using namespace std ;

static int NODE_NUM ;

static string request_kind ;

bool write(double , string) ;

int main(int argc , char* argv[])// ./log-analysis request-kind topology(CERNET2/Deltacom/GtsCe/Oteglobe) cache-strategy(PPDS/LRU/no-cache/LRU-random)
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
	if(cache_strategy != "PPDS" && cache_strategy != "LRU" && cache_strategy != "no-cache" && cache_strategy != "LRU-random")
	{
		cout<<"log-analysis:para 4(cache strategy) wrong. only PPDS/LRU/no-cache/LRU-random is supported"<<endl ;
		return 0 ;
	}

	ifstream file ;
	int req_num = 0 ;
	int hit_num = 0 , rep_num = 0 , cache_num = 0 , hop_sum = 0 ;
	double req_num_weight = 0 ;
	double hit_num_weight = 0 , hop_sum_weight = 0 ;
	for(int i = 1 ; i <= NODE_NUM ; ++i)
	{
		file.open("./source/log/node" + to_string(i)) ;
		if(!file.is_open())
		{
			cout<<"./source/log/node"<<i<<" open failed in log-analysis."<<endl ;
			return 0 ;
		}
		
		string str ;
		while(file>>str)
		{
			if(str == "get")
			{
				++req_num ;
				int content_size(0) ;
				file>>str>>content_size>>str ;
				req_num_weight += content_size ;
				if(str == "cache")
				{
					++hit_num ;
					hit_num_weight += content_size ;
				}
				int hop ;
				file>>hop ;
				hop_sum += hop ;
				hop_sum_weight += (hop*content_size) ;
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n') ;
			}
			else if(str == "replace")
			{
				++rep_num ;
				++cache_num ;
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n') ;
			}
			else if(str == "store")
			{
				++cache_num ;
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n') ;
			}
			else
			{
				//...
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n') ;
			}
		}

		file.close() ;
	}
	write(1.0 * hit_num / req_num , "./doc/data/" + topology + ".topology/" + cache_strategy  + "/cache-hit-rate") ;
	write(1.0 * rep_num / cache_num , "./doc/data/" + topology + ".topology/" + cache_strategy  + "/cache-replace-rate") ;
	write(1.0 * hop_sum / req_num , "./doc/data/" + topology + ".topology/" + cache_strategy  + "/avg-req-dist") ;	

	write(hit_num_weight / req_num_weight , "./doc/data/" + topology + ".topology/" + cache_strategy  + "/cache-hit-rate-weight") ;
	write(hop_sum_weight / req_num , "./doc/data/" + topology + ".topology/" + cache_strategy  + "/avg-req-dist-weight") ;	
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