/*实现从NFA到DFA的转化
由用户输入自动机 M = (Q, T, δ，q0, F)
用stl容器set存储
对δ构建一个结构来存储
set的用法：https://cloud.tencent.com/developer/article/2301483
从q0开始，在δ中寻找从q0开始所能到达的状态，存入集合中，若包含终结状态，则标记为终结状态
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
