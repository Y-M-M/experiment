#!/bin/bash
touch res.txt
g++ -o start.o step3/main.cpp								#用gcc编译生成可执行文件

read -p '' typ											#读取键盘输入到typ中

./start.o step3/test_set/test$typ.txt > res.txt            #输入测试文件，输出到res.txt
cat res.txt
