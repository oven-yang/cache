#include"../header/PopularityTable.h"

PopularityTable::PopularityTable() : capacity(POPULARITY_DEFAULT_CAPACITY) , sum(0)
{
    //
}

PopularityTable::PopularityTable(unsigned capacity) : capacity(capacity) , sum(0)
{
    //
}

unsigned PopularityTable::getQuantity(ContentName name)
{
    if(receive_table.count(name) == 1)
    {
        return receive_table[name] ;
    }
    else
    {
        return 0 ;
    }
}

double PopularityTable::getRate(ContentName name)
{
    return 1.0*getQuantity(name)/sum ;
}

unsigned PopularityTable::updateQuantity(ContentName name)
{
    if(receive_table.count(name) == 0)
    {
        if(receive_table.size() == capacity)
        {
            return 0 ;
        }
        else
        {
            receive_table[name] = 1 ;
        }
    }
    else
    {
        ++receive_table[name] ;
    }
    ++sum ;
    return receive_table[name] ;
}

bool PopularityTable::setCapacity(unsigned new_capacity)
{
    if(new_capacity < receive_table.size())
    {
        return false ;
    }
    capacity = new_capacity ;
    return true ;
}

void PopularityTable::clear()
{
    receive_table.clear() ;
    sum = 0 ;
}