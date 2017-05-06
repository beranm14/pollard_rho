#!/bin/bash

i=1;

out="data_v2.txt";
echo -n > $out;

while [[ $i -le 1024 ]];do
	j=1;
	while [[ $j -le 1024 ]];do
		drl="../benchmarks_v2/logs_gtx_780_Ti_pol/log_${i}_${j}/";
		if [[ -d $drl ]];then
			tml=`grep "Time: " ${drl}gpu_job_del.sh.o* | cut -d' ' -f 2`;
			echo $i $j $tml;
			[[ ! -z "$tml" ]] && { echo $i $j $tml >> $out; };
		fi
		j=$(($j+1));
	done;
	i=$(($i+1));
done;

