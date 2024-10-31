#!/bin/bash
#get file list
filelist=$(ls ./sample)

for file in $filelist
do
    echo $filelist
    ./pp ./sample/$file
done

./pp ./sample/1.txt
./pp