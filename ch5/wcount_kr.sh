#!/bin/sh

for f in robi*.txt
do 
./kr.pl $f | sort | uniq -c | sort -rg > $f.w 
done

for f in speed*.txt
do 
./kr.pl $f | sort | uniq -c | sort -rg > $f.w 
done

