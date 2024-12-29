#!/bin/bash
#get file list
filelist=$(ls ./sample)

for file in $filelist
do
    ./cr ./sample/$file
done


./cr ./sample/1.txt
./cr