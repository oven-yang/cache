#include"../header/PreferenceTable.h"

PreferenceTable::PreferenceTable() : capacity(PREFERENCE_DEFAULT_CAPACITY) , sum(0)
{
}

PreferenceTable::PreferenceTable(unsigned capacity) : capacity(capacity) , sum(0)
{
}

unsigned PreferenceTable::getQuantity(ContentName name)
{
    return send_table[name.getContentType()] ;
}

double PreferenceTable::getRate(ContentName name)
{
    return 1.0*getQuantity(name)/sum ;
}

unsigned PreferenceTable::updateQuantity(ContentName name)
{
    ++sum ;
    return ++send_table[name.getContentType()] ;
}

void PreferenceTable::clear()
{
    for(map<ContentType , unsigned>::iterator it = send_table.begin() ; it != send_table.end() ; ++it)
    {
        it->second = 0 ;
    }
    sum = 0 ;
}

bool PreferenceTable::setCapacity(unsigned new_capacity)
{
    if(new_capacity < send_table.size())
    {
        return false ;
    }
    capacity = new_capacity ;
    return true ;
}