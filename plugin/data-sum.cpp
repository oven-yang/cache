#include<iostream>
#include<fstream>
#include<string>
#include<cstring>

using namespace std ;

bool sum(string index)
{
    ifstream cernet2 , deltacom , gtsce , oteglobe ;
    ofstream  out ;
    string read ;

    out.open("./doc/data/all.topology/" + index , ios::out | ios::trunc) ;
    cernet2.open("./doc/data/CERNET2.topology/" + index) ;
    deltacom.open("./doc/data/Deltacom.topology/" + index) ;
    gtsce.open("./doc/data/GtsCe.topology/" + index) ;
    oteglobe.open("./doc/data/Oteglobe.topology/" + index) ;

    out<<"quantity CERNET2 Deltacom GtsCe Oteglobe\n" ;

    for(int i = 10 ; i < 21 ; ++i)
    {
        out<<i ;
        cernet2>>read>>read ;
        out<<" "<<read ;
        deltacom>>read>>read ;
        out<<" "<<read ;
        gtsce>>read>>read ;
        out<<" "<<read ;
        oteglobe>>read>>read ;
        out<<" "<<(i != 20 ? (read + "\n") : read ) ;
    }

    cernet2.close() ;
    gtsce.close() ;
    deltacom.close() ;
    oteglobe.close() ;
    out.close() ;
    return true ;
}

int main(void)
{
    sum("avg-req-dist") ;
    sum("cache-hit-rate") ;
    sum("cache-replace-rate") ;
    return 0 ;
}