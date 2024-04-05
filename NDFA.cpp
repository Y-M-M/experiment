/*实现从NFA到DFA的转化
由用户输入自动机 M = (Q, T, δ，q0, F)
用stl容器set存储
对δ构建一个结构来存储，包括前一个状态、后一个状态和移动字符*/
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <utility>
#include <map>
#include <set>
#include <vector>
using namespace std;
