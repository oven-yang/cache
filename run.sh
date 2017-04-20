#!/bin/bash
make init-log-analysis
make init-node
rm -f ./source/log/*
topologyarr=("CERNET2" "Deltacom" "GtsCe" "Oteglobe")
nodes=(20 113 149 93)
strategyarr=("PPDS" "LRU" "no-cache")
i=0
while [ $i -lt ${#topologyarr[*]} ]
do
	j=0
	while [ $j -lt ${#strategyarr[*]} ]
	do
        rm -f ./doc/data/${topologyarr[$i]}.topology/${strategyarr[$j]}/*
        req=10
        while [ $req -lt 21 ]
        do    
            rm -f ./source/backup-log/${topologyarr[$i]}.topology/${strategyarr[$j]}/$req/*
            req=`expr $req + 1`
        done
		j=`expr $j + 1`
	done
	i=`expr $i + 1`
done
echo "old data removed"
i=0
while [ $i -lt ${#topologyarr[*]} ]
do
    req=10
    while [ $req -lt 21 ]
    do
        rm `ls ./source | awk '{if($0~"node[1-9]+") print "./source/"$0}'`
        ./node-define ${nodes[${i}]} $req
        j=0
        while [ $j -lt ${#strategyarr[*]} ]
        do
            ./cache ${topologyarr[${i}]} ${strategyarr[${j}]}
            ./log-analysis $req ${topologyarr[${i}]} ${strategyarr[${j}]}
            mv ./source/log/* ./source/backup-log/${topologyarr[$i]}.topology/${strategyarr[$j]}/$req
            echo "$req ${topologyarr[${i}]} ${strategyarr[${j}]} finished"
            j=`expr $j + 1`
        done
        req=`expr $req + 1`
    done
    i=`expr $i + 1`
done