#include<vector>
#include<string>
#include<stdexcept>
#include<regex>
#include"../header/ContentName.h"

using std::string ;

ContentName::ContentName()
{
	//
}
ContentName::ContentName(string new_name)//string should be seperated with '/'
{
	if(!std::regex_match(new_name , std::regex("(([a-z]{1,})/)*([a-z]{1,})")))
	{
		throw std::runtime_error("ContentName constructor argument illegal, not match regex:" + new_name) ;
	}
	string::size_type start = 0 , end = new_name.find_first_of('/' , start) ;
	while(end != string::npos)
	{
		name.push_back(new_name.substr(start , end - start)) ;
		end = new_name.find_first_of('/' , start = end + 1) ;
	}
	if(start == string::npos || start >= new_name.size())
	{
		throw std::runtime_error("ContentName constructor argument illegal, start overflow:" + new_name) ;
	}
	name.push_back(new_name.substr(start)) ;
}
string ContentName::getStrName() const
{
	string str ;
	for(string::size_type i = 0 ; i != name.size() ; ++i)
	{
		str += (name[i] + "/") ;
	}
	return str.substr(0 , str.size() - 1) ;
}

ContentType ContentName::getContentType()//此处实现不合理,扩展性差,可以定义一个全局map常量.
{
	for(vector<string>::size_type i = 0 ; i < name.size() ; ++i)
	{
		if(name[i] == video_type_key)
		{
			return VideoType ;
		}
		else if(name[i] == file_type_key)
		{
			return FileType ;
		}
		else if(name[i] == audio_type_key)
		{
			return AudioType ;
		}
	}
	return UndefinedType ;
}
