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
timer update_circle = 20 ;//缓存更新周期
double pref_weight = 0.2 ;
double popu_weight = 0.8 ;
double old_weight = 0.5 ;
string cache_strategy("not-defined") ;
double min_priority = 0 ;

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
	//i do not want this code.
	//and i think it will cause bad performance.
	// double size_cal , h_cs = 0.01 ;
	// double cs_l = 1<<10 , cs_h = 1<<20 ;
	// if(size >= cs_h)
	// {
	// 	size_cal = h_cs ;
	// }
	// else if(size <= cs_l)
	// {
	// 	size_cal = 1 ;
	// }
	// else
	// {
	// 	size_cal = (cs_h - h_cs*cs_l - (1-h_cs)*size)/(cs_h - cs_l) ;//wtf
	// }
	// return size_cal ;
	//remove above if you can

	// this is mine.
	return basic_cache_priority/size ;
}