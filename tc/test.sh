#!/bin/bash

#get file list
filelist=$(ls ./sample)
#run ./tc for each file
for file in $filelist
do
    ./tc ./sample/$file
done
# less than 2 arguments
./tc 
# not exist test file
./tc ./sample/1.txt