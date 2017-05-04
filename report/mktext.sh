#!/bin/bash
[[ ! -r "$1" ]] && exit 10;
b=`basename $1`;
[[ "`echo $b | cut -d. -f2`" != "tex"  ]] && exit 11;
bs="`echo $b | cut -d. -f1`";

i=0;
while [[ $i -lt 3 ]];do
	bibtex "$bs" || exit 12;
	pdflatex "$bs" || exit 12;
	i=$(($i+1));
	echo $i
done

