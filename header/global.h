#ifndef GLOBAL_H
#define GLOBAL_H

#include<string>
#include<vector>

using std::string ;
using std::vector ;

class Node ;

/********************************************************************************
 **************************   new type definition   *****************************
 *caution: 定义的类名,变量名,不要与枚举关键字冲突
 ********************************************************************************/

//计时器类型,当需要时可定义timer类,记得删除此处的定义,并找到所有引用此
//类型的地方,确保新定义的timer类支持所有引用timer类处使用的操作和运算
typedef unsigned timer ;

//define interafce state
enum InterfaceState{closed , open , linked , transmitting} ;

//define packet type
enum PacketType{InterestPacketType , DataPacketType} ;

//内容分类
enum ContentType{FileType , VideoType , AudioType , UndefinedType} ;


/********************************************************************************
 ************************   function definition   *******************************
 ********************************************************************************/

string int_to_str(unsigned num) ;

unsigned str_to_int(string str) ;

void log(string node_name , string record) ;
void log(string record) ;

void floyd(vector<vector<int> >& graph , vector<vector<int> >& dist ,  vector<vector<int> >& path) ;

double calculateCachePriority(double basic_cache_priority , unsigned size) ;


/********************************************************************************
 ***************************   global variable   ********************************
 ********************************************************************************/

const unsigned PIT_DEFAULT_CAPACITY = 100000 ;
const unsigned FIB_DEFAULT_CAPACITY = 100000 ;
const unsigned CS_DEFAULT_CAPACITY  = 100000 ;
const unsigned POPULARITY_DEFAULT_CAPACITY = 100000 ;
const unsigned PREFERENCE_DEFAULT_CAPACITY = 100000 ;
const timer period = 100 ;
const string file_type_key("file") ;
const string video_type_key("video") ;
const string audio_type_key("audio") ;
extern timer global_clock ;
extern timer update_circle ;//缓存更新周期
extern double pref_weight ;
extern double popu_weight ;
extern double old_weight  ;

#endif
