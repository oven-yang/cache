#
#工程文件夹 header 和 src 中未在此文件说明的源文件皆为无效文件.
#
vpath %.h ./header
vpath %.cpp ./src
vpath %.o ./bin

objs = main.o ContentName.o global.o ContentStore.o DataPacket.o \
          InterestPacket.o PendingInterestTable.o \
          ForwardingInterestBase.o Interface.o \
		  Node.o PreferenceTable.o PopularityTable.o \
		  RoutingTableItem.o RoutingTable.o
objects = $(foreach i , $(objs) , ./bin/$(i))


cache : $(objs)
	clang++ -g -std=c++11 $(objects) -o cache

main.o : main.cpp
	clang++ -g -std=c++11 -c src/main.cpp -o ./bin/$@

RoutingTable.o : RoutingTable.cpp RoutingTable.h RoutingTableItem.h ContentName.h
	clang++ -g -std=c++11 -c src/RoutingTable.cpp  -o ./bin/$@

RoutingTableItem.o : RoutingTableItem.cpp RoutingTableItem.h Interface.h
	clang++ -g -std=c++11 -c src/RoutingTableItem.cpp -o ./bin/$@

Node.o : Node.cpp Node.h Interface.h global.h ContentStore.h ForwardingInterestBase.h \
         PendingInterestTable.h DataPacket.h InterestPacket.h RoutingTable.h
	clang++ -g -std=c++11 -c src/Node.cpp -o ./bin/$@

PopularityTable.o : PopularityTable.cpp PopularityTable.h ContentName.h
	clang++ -g -std=c++11 -c src/PopularityTable.cpp -o ./bin/$@

PreferenceTable.o : PreferenceTable.cpp PreferenceTable.h global.h ContentName.h
	clang++ -g -std=c++11 -c src/PreferenceTable.cpp -o ./bin/$@

InterestPacket.o : InterestPacket.cpp InterestPacket.h ContentName.h 
	clang++ -g -std=c++11 -c src/InterestPacket.cpp -o ./bin/$@

DataPacket.o : DataPacket.cpp DataPacket.h ContentName.h
	clang++ -g -std=c++11 -c src/DataPacket.cpp -o ./bin/$@

ContentStore.o : ContentStore.cpp ContentStore.h
	clang++ -g -std=c++11 -c src/ContentStore.cpp -o ./bin/$@

PendingInterestTable.o : PendingInterestTable.cpp PendingInterestTable.h Interface.h ContentName.h InterestPacket.h
	clang++ -g -std=c++11 -c src/PendingInterestTable.cpp -o ./bin/$@

ForwardingInterestBase.o : ForwardingInterestBase.cpp ForwardingInterestBase.h Interface.h ContentName.h InterestPacket.h
	clang++ -g -std=c++11 -c src/ForwardingInterestBase.cpp -o ./bin/$@

Interface.o : Interface.cpp Interface.h global.h
	clang++ -g -std=c++11 -c src/Interface.cpp -o ./bin/$@

ContentName.o : ContentName.cpp ContentName.h
	clang++ -g -std=c++11 -c src/ContentName.cpp -o ./bin/$@

global.o : global.cpp global.h
	clang++ -g -std=c++11 -c src/global.cpp -o ./bin/$@

.PHONY : clean
clean :
	-rm cache $(objects)

.PHONY : deletelog
deletelog :
	-rm ./source/log/*

.PHONY : backup-log
backup-log :
	-rm ./log-backup/*
	-mv ./source/log/* ./log-backup/

.PHONY : init-node
init-node :
	g++ -o node-define plugin/node-define.cpp

.PHONY : init-log-analysis
init-log-analysis :
	g++ -o log-analysis plugin/log-analysis.cpp

.PHONY : delete-conclusion
delete-conclusion :
	-rm ./conclusion/*
