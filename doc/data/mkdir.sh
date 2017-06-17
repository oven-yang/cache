#!/bin/bash
topologyarr=("CERNET2" "Deltacom" "GtsCe" "Oteglobe")
strategyarr=("PPDS" "LRU" "no-cache" "LRU-random")
i=0
while [ $i -lt ${#topologyarr[*]} ]
do
	j=0
	while [ $j -lt ${#strategyarr[*]} ]
	do
		touch ./${topologyarr[$i]}.topology/${strategyarr[$j]}/avg-req-dist-weight \
		      ./${topologyarr[$i]}.topology/${strategyarr[$j]}/cache-hit-rate-weight \
			  ./${topologyarr[$i]}.topology/${strategyarr[$j]}/avg-req-dist \
			  ./${topologyarr[$i]}.topology/${strategyarr[$j]}/cache-hit-rate \
			  ./${topologyarr[$i]}.topology/${strategyarr[$j]}/cache-replace-rate
		j=`expr $j + 1`
	done
    i=`expr $i + 1`
done
