#!/bin/bash
#get file list
filelist=$(ls ./sample)

for file in $filelist
do
    ./pp ./sample/$file
done


./pp ./sample/1.txt
./pp