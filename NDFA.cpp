/*实现从NFA到DFA的转化
由用户输入自动机 M = (Q, T, δ，q0, F)
数组存储Q、T&δ，单独存储q0和F
T&δ作为结构体，内容包括字母表、和该字母表对应的转移函数
set的用法：https://cloud.tencent.com/developer/article/2301483
用数组存储新状态的集合
从q0开始，在δ中寻找从q0开始所能到达的状态，存入集合中
*/
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <utility>
#include <map>
#include <set>
#include <vector>
using namespace std;

int main(){

}