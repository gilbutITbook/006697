#!/bin/sh

for f in alice*.txt
do 
./getword < $f | sort | uniq -c | sort -rg > $f.w 
done

