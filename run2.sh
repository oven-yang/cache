min=1
max=5

i=$min
while [ $i -le $max ]
do
    ./run.sh
    mkdir conclusion/data$i
    cp doc/data/* conclusion/data$i -r
    i=`expr $i + 1`
done