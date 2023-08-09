#by MOBI

#!/bin/bash
g++ main.cpp
for i in $(seq 1 5) 
do  
    cp ./Test_Cases/$i/scene.txt ./scene.txt
    ./a.out ./Test_Cases/$i/scene.txt $i
    diff -EZBwb ./Test_Cases/$i/stage1.txt ./stage1.txt
    diff -EZBwb ./Test_Cases/$i/stage2.txt ./stage2.txt
    diff -EZBwb ./Test_Cases/$i/stage3.txt ./stage3.txt
    diff -EZBwb ./Test_Cases/$i/z_buffer.txt ./z-buffer.txt
done
