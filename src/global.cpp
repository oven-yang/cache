#include"../header/global.h"
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>

using std::string ;
using std::to_string ;
using std::ofstream ;
using std::endl ;

//global variable

timer global_clock = 0 ;
timer update_circle = 1 ;//缓存更新周期
double pref_weight = 0.4 ;
double popu_weight = 0.5 ;
double old_weight = 0.8 ;
string cache_strategy("not-defined") ;

//functions
string int_to_str(unsigned num)
{
	return to_string(num) ;
}

unsigned str_to_int(string str)
{
	std::istringstream stream(str);
	unsigned num ;
	stream>>num ;
	return num ;
}

void log(string node_name , string record)
{
	ofstream out("./source/log/" + node_name , std::ios::app) ;
	if(out.is_open() == false)
	{
		std::cout<<endl<<endl<<"-------------------------log file open failed."<<endl<<endl ;
	}
	out<<record + " " + int_to_str(global_clock)<<std::endl ;
	out.flush() ;
	out.close() ;
}

void log(string record)
{
	ofstream out("./source/log/main" , std::ios::app) ;
	if(out.is_open() == false)
	{
		std::cout<<endl<<endl<<"-------------------------log file open failed."<<endl<<endl ;
	}
	out<<record + " " + int_to_str(global_clock)<<std::endl ;
	out.close() ;
}

void floyd(vector<vector<int> >& graph , vector<vector<int> >& dist ,  vector<vector<int> >& path)
{
	
}

double calculateCachePriority(double basic_cache_priority , unsigned size)
{
	return basic_cache_priority/size ;
}