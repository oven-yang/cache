#
#工程文件夹 header 和 src 中未在此文件说明的源文件皆为无效文件.
#
vpath %.h header
vpath %.cpp src:plugin
vpath %.o bin

$(shell gtags)

objs = main.o ContentName.o global.o ContentStore.o DataPacket.o \
          InterestPacket.o PendingInterestTable.o \
          Interface.o Node.o PreferenceTable.o PopularityTable.o \
		  RoutingTableItem.o RoutingTable.o
objects = $(foreach i , $(objs) , ./bin/$(i))

cache : $(objs)
	g++ -g -std=c++11 $(objects) -o cache

main.o : main.cpp
	g++ -g -std=c++11 -c src/main.cpp -o ./bin/$@

RoutingTable.o : RoutingTable.cpp RoutingTable.h RoutingTableItem.h ContentName.h
	g++ -g -std=c++11 -c src/RoutingTable.cpp  -o ./bin/$@

RoutingTableItem.o : RoutingTableItem.cpp RoutingTableItem.h Interface.h
	g++ -g -std=c++11 -c src/RoutingTableItem.cpp -o ./bin/$@

Node.o : Node.cpp Node.h Interface.h global.h ContentStore.h \
         PendingInterestTable.h DataPacket.h InterestPacket.h RoutingTable.h
	g++ -g -std=c++11 -c src/Node.cpp -o ./bin/$@

PopularityTable.o : PopularityTable.cpp PopularityTable.h ContentName.h
	g++ -g -std=c++11 -c src/PopularityTable.cpp -o ./bin/$@

PreferenceTable.o : PreferenceTable.cpp PreferenceTable.h global.h ContentName.h
	g++ -g -std=c++11 -c src/PreferenceTable.cpp -o ./bin/$@

InterestPacket.o : InterestPacket.cpp InterestPacket.h ContentName.h 
	g++ -g -std=c++11 -c src/InterestPacket.cpp -o ./bin/$@

DataPacket.o : DataPacket.cpp DataPacket.h ContentName.h
	g++ -g -std=c++11 -c src/DataPacket.cpp -o ./bin/$@

ContentStore.o : ContentStore.cpp ContentStore.h
	g++ -g -std=c++11 -c src/ContentStore.cpp -o ./bin/$@

PendingInterestTable.o : PendingInterestTable.cpp PendingInterestTable.h Interface.h ContentName.h InterestPacket.h
	g++ -g -std=c++11 -c src/PendingInterestTable.cpp -o ./bin/$@

Interface.o : Interface.cpp Interface.h global.h
	g++ -g -std=c++11 -c src/Interface.cpp -o ./bin/$@

ContentName.o : ContentName.cpp ContentName.h
	g++ -g -std=c++11 -c src/ContentName.cpp -o ./bin/$@

global.o : global.cpp global.h
	g++ -g -std=c++11 -c src/global.cpp -o ./bin/$@

.PHONY : clean
clean :
	-rm cache $(objects) node-define log-analysis

.PHONY : deletelog
deletelog :
	-rm ./source/log/*

.PHONY : node-define
node-define :
	g++ -g -std=c++11 plugin/node-define.cpp -o node-define

.PHONY : log-analysis
log-analysis :
	g++ -g -std=c++11 plugin/log-analysis.cpp -o log-analysis