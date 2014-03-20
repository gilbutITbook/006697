#!/bin/sh

for f in toshi*.txt
do 
./jp.pl < $f | sort -rg > $f.n 
done

