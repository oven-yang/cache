#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<limits>

using namespace std ;

int link[200][200] ;
int line(0) ;
static int node_num(0) ;

int main(int argc , char *argv[])//exec ./gml2link gmlfile linkfile
{
    if(argc != 3)
    {
        cout<<"argc error"<<endl ;
        return 0 ;
    }
    memset(link , 0 , sizeof link) ;

    ifstream in(argv[1]) ;
    if(!in.is_open())
    {
        cout<<"gmlfile open failed."<<endl ;
        return 0 ;
    }
    string key ;
    while(in>>key)//read
    {
        ++line ;
        if(key == "node")
        {
            ++node_num ;
            in.ignore(numeric_limits<std::streamsize>::max() , '\n') ;
            string subkey ;
            int subval ;
            while(in>>subkey && subkey != "]")
            {
                ++line ;
                if(subkey == "id")
                {
                    if(in>>subval && subval < 200)
                    {
                        link[subval][subval] = 1 ;
                    }
                }
                else
                {
                    in.ignore(numeric_limits<std::streamsize>::max() , '\n') ;
                }
            }
            if(subkey != "]")
            {
                cout<<"line "<<line<<" error"<<endl ;
                return 0 ;
            }
        }
        else if(key == "edge")
        {
            in.ignore(numeric_limits<std::streamsize>::max() , '\n') ;
            string subkey ;
            int src(-1) , dest(-1) ;
            while(in>>subkey && subkey != "]")
            {
                ++line ;
                if(subkey == "source")
                {
                    in>>src ;
                }
                else if(subkey == "target")
                {
                    in>>dest ;
                }
                else
                {
                    in.ignore(numeric_limits<std::streamsize>::max() , '\n') ;
                }
            }
            if(subkey != "]")
            {
                cout<<"line "<<line<<" error"<<endl ;
                return 0 ;
            }
            if(src != -1 && dest != -1 && src < 200 && dest < 200 && link[src][src] == link[dest][dest] == 1)
            {
                link[src][dest] = link[dest][src] = 1 ;
            }
            else
            {
                cout<<"line "<<line<<" edge error"<<endl ;
            }
        }
        else if(key != "]")
        {
            in.ignore(numeric_limits<std::streamsize>::max() , '\n') ;
        }
    }
    in.close() ;

    cout<<node_num<<" nodes"<<endl ;
    
    ofstream out(argv[2] , ios::out | ios::trunc) ;
    if(!out.is_open())
    {
        cout<<"linkfile open failed."<<endl ;
        return 0 ;
    }
    for(int i = 1 ; i < 200 ; ++i)
    {
        for(int j = 0 ; j < i ; ++j)
        {
            if(link[i][j] == 1)
            {
                out<<"node"<<i+1<<" node"<<j+1<<"\n" ;
            }
        }
    }
    out.close() ;

    return 0 ;
}