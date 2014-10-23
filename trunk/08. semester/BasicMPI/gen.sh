#!bin/bash

NUM=0
for i in 01 02 03 04 05 06 07 08 09 10 21 22 23 24 31 32
do
	((NUM++))
	ssh w$i 'echo '$i' > /tmp/num.txt'
	echo $i
done
