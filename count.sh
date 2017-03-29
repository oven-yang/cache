#!/bin/zsh
i=1
while [ $i -lt 21 ]
do
	echo "node$i \c"
	cat source/log/node$i | grep get | grep -v error | wc -l
	i=`expr $i + 1`
done
