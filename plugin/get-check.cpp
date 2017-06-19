#include<limits>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<utility>
#include<algorithm>

using namespace std ;

int main(int argc , char *argv[]) // ./get-check node-name
{
    if(argc != 2)
    {
        cout<<"para error"<<endl ;
        return 0 ;
    }
    string node(argv[1]) ;
    vector<pair<int , string> > ops ;
    ifstream file ;

    file.open("./source/" + node) ;
    if(!file.is_open())
    {
        cout<<"./source/log/"<<node<<" open failed"<<endl ;
        return 0 ;
    }
    int timer ;
    string op ;
    while(file>>op && op != "[operation]") ;
    while(file>>timer>>op) ops.push_back(make_pair(timer , op)) ;
    file.close() ;

    file.open("./source/log/" + node) ;
    if(!file.is_open())
    {
        cout<<"./source/log/"<<node<<" open failed"<<endl ;
        return 0 ;
    }
    cout<<"\n-------------404---------------\n" ;
    while(file>>op)
    {
        if(op == "get")
        {
            file>>op ;
            auto it = ops.begin() ;
            for(; it != ops.end() && it->second != op ; ++it) ;
            if(it != ops.end()) ops.erase(it) ;
            else cout<<op<<endl ;
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n') ;
    }
    cout<<"-------------404---------------\n\n" ;
    file.close() ;

    for(int i = 0 ; i < ops.size() ; ++i)
    {
        cout<<ops[i].first<<" "<<ops[i].second<<endl ;
    }
    return 0 ;
}