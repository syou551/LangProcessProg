#!/bin/bash

#get file list
filelist=$(ls ./sample)
#run ./tc for each file
for file in $filelist
do
    echo $file
    ./pp ./sample/$file
done