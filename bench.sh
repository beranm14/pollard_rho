#!/bin/bash

# Smartass script eating whole server resources
# It just give different block count and thread count
# and estimates the fastest one.
echo "" > log.txt;

for blocks in `seq 1 4 1024`;do
	for threads in `seq 1 4 1024`;do
		elaps=`./try 1 $blocks $threads | grep "Time: " | awk '{print $2}'`;
		echo $blocks $threads $elaps >> log.txt;
		echo Done $blocks x $threads : $elaps;
	done
done
