#by MOBI

#!/bin/bash
g++ main.cpp
for i in $(seq 1 4) 
do
    ./a.out ./Test_Cases/$i/scene.txt
    diff -EZBwb ./Test_Cases/$i/stage1.txt ./stage1.txt
    diff -EZBwb ./Test_Cases/$i/stage2.txt ./stage2.txt
    diff -EZBwb ./Test_Cases/$i/stage3.txt ./stage3.txt
done
