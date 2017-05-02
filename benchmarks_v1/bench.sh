#!/bin/bash

# Smartass script eating whole server resources
# It just give different block count and thread count
# and estimates the fastest one.
echo "" > log_gpu1.txt;

for blocks in `seq 10 10 1024`;do
	for threads in `seq 10 10 1024`;do
		elaps=`./try 1 $blocks $threads | grep "Time: " | awk '{print $2}' | tee out.txt`;
		echo $blocks $threads $elaps >> log_gpu1.txt;
		echo Done $blocks x $threads : $elaps;
	done
done
