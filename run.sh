#!/bin/bash
make
make init-log-analysis
make init-node
rm -f ./source/log/*

minoperation=10000
maxoperation=20000
opplus=1000
minkind=1000
maxkind=1009
topologyarr=("CERNET2" "Deltacom" "GtsCe" "Oteglobe")
nodes=(20 113 149 93)
strategyarr=("PPDS" "LRU" "no-cache" "LRU-random")
#strategyarr=("PPDS" "LRU")

i=0
while [ $i -lt ${#topologyarr[*]} ]
do
	j=0
	while [ $j -lt ${#strategyarr[*]} ]
	do
        rm -f ./doc/data/${topologyarr[$i]}.topology/${strategyarr[$j]}/*
        rm -r ./source/backup-log/${topologyarr[$i]}.topology/${strategyarr[$j]}/*
        req=$minoperation
        while [ $req -lt $maxoperation ]
        do
            mkdir ./source/backup-log/${topologyarr[$i]}.topology/${strategyarr[$j]}/$req
            req=`expr $req + $opplus`
        done
		j=`expr $j + 1`
	done
	i=`expr $i + 1`
done
echo "old data removed"

i=0
while [ $i -lt ${#topologyarr[*]} ]
do
    req=$minoperation
    while [ $req -lt $maxoperation ]
    do
        rm `ls ./source | awk '{if($0~"node[1-9]+") print "./source/"$0}'`
        ./node-define ${nodes[${i}]} $minkind $req
        j=0
        while [ $j -lt ${#strategyarr[*]} ]
        do
            ./cache ${topologyarr[${i}]} ${strategyarr[${j}]}
            ./log-analysis $req ${topologyarr[${i}]} ${strategyarr[${j}]}
            mv ./source/log/* ./source/backup-log/${topologyarr[$i]}.topology/${strategyarr[$j]}/$req
            echo "$req ${topologyarr[${i}]} ${strategyarr[${j}]} finished"
            j=`expr $j + 1`
        done
        req=`expr $req + $opplus`
    done
    i=`expr $i + 1`
done
