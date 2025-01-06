#!/bin/bash
#get file list
filelist=$(ls ./sample)

for file in $filelist
do
    ./mpplc ./sample/$file
done

filelist=$(ls /lpp_test/coverage/)

for file in $filelist
do
    ./mpplc /lpp_test/coverage/$file
done

./mpplc ./sample/1.txt
./mpplc