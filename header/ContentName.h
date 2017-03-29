#ifndef CONTENT_NAME_H
#define CONTENT_NAME_H

#include"global.h"
#include<vector>
#include<string>

using std::vector ;
using std::string ;

class ContentName
{
private:
	vector<string> name ;
public:
	ContentName() ;
	ContentName(string new_name) ;//string should be seperated with '/'
	// void str_to_name(string str) ;
	string getStrName() const ;
	vector<string> getFormedName() const { return name ; }
	ContentType getContentType() ;

	inline bool operator==(const ContentName &contentName1) const
	{
		return this->getStrName() == contentName1.getStrName() ;
	}
	inline bool operator!=(const ContentName &contentName1) const
	{
		return this->getStrName() != contentName1.getStrName() ;
	}
	inline ContentName& operator=(const ContentName contentName1)
	{
		name.clear() ;
		name = contentName1.getFormedName() ;
		return *this ;
	}
	inline bool operator<(const ContentName &contentName1) const
	{
		return name < contentName1.getFormedName() ;
	}
} ;

#endif
