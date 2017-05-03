#!/bin/bash
mkdir log;
for blocks in `seq 10 100 1024`;do
        for threads in `seq 10 100 1024`;do
                cat gpu_long_job.sh | sed -e "s/BLOCKS/${blocks}/g"  -e "s/THREADS/${threads}/g" > gpu_job_del.sh;

                while [[ `qstat | grep beranm14` != "" ]]; do
                        sleep 1;
                        echo "Waiting for ${blocks} ${threads}";
                done

        	qsub gpu_job_del.sh;

                while [[ `qstat | grep beranm14` != "" ]]; do
                        sleep 1;
                        echo "Waiting for ${blocks} ${threads}";
                done
        	sleep 1;

        	mv log logs_gtx_780_Ti_pol/log_${blocks}_${threads};
        	mkdir log
        done
done
rm log;
[[ -f gpu_job_del.sh ]] && { rm gpu_job_del.sh; };
