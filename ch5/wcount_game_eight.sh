#!/bin/sh

for f in game*.txt
do 
./getword < $f | sort | uniq -c | sort -rg > $f.w 
done

for f in eight*.txt
do
./getword < $f | sort | uniq -c | sort -rg > $f.w
done