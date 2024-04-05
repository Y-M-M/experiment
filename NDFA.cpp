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


typedef struct transfer{
    char sign;
    char next;
}Transfer;
typedef struct Q{
    char state;
    vector<Transfer> trans;
}Qs;

char read()//忽略空格和回车
{
    char c = getchar();
    while(c == ' ' || c == '\n') c = getchar();
    return c;
}

int main()
{
    printf("本程序完成从NFA到DFA的转化，接下来请您输入需要进行转化的NFA：\n");
    printf("请输入NFA状态的个数：\n");
    int states_num = 0;
    scanf("%d", &states_num);
    printf("请依次输入状态名称，以空格或回车分隔不同状态(请保证状态名称是单个字符)：\n");
    Qs state[states_num];
    for(int i = 0; i < states_num; i++)  state[i].state = read();
    printf("请输入NFA字母表的个数：\n");
    int sign_num = 0;
    scanf("%d", &sign_num);
    char signs[sign_num];
    printf("请依次输入NFA包含的字符，以空格或回车分隔不同的状态：\n");
    for(int i = 0; i < sign_num; i++) signs[i] = read(); 
    
}