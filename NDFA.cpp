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

void readNFA(int &states_num, Qs *state, int &sign_num, char *signs, int &delta_num, char &q0, int &F_num, char *F)
{
    printf("本程序完成从NFA到DFA的转化，接下来请您输入需要进行转化的NFA：\n");
    //读取Q
    printf("请输入NFA状态的个数：\n");
    scanf("%d", &states_num);
    printf("请依次输入状态名称，以空格或回车分隔不同状态(请保证状态名称是单个字符)：\n");
    for(int i = 0; i < states_num; i++)  state[i].state = read();
    //读取T
    printf("请输入NFA字母表的个数：\n");
    scanf("%d", &sign_num);
    printf("请依次输入NFA包含的字符，以空格或回车分隔不同的状态：\n");
    for(int i = 0; i < sign_num; i++) signs[i] = read(); 
    //读取δ
    printf("请输入您将输入的转移函数的个数：\n");
    scanf("%d", &delta_num);
    printf("请输入转移函数，格式为p(当前状态) q(转移字符) r(下一个状态)，用空格或回车分隔");
    for(int i = 0; i < delta_num; i++)
    {
        char p, q, r;
        p = read();
        q = read();
        r = read();
        for(int j = 0; j < states_num; j++)
        {
            if(state[j].state == p)  state[j].trans.push_back({q, r});
        }
    }
    //读取q0
    printf("请输入起始状态q0：\n");
    q0 = read();
    //读取T
    printf("请输入终结状态T的数目：\n");
    scanf("%d", &F_num);
    printf("请依次输入终结状态：\n");
    for(int i = 0; i < F_num; i++) F[i] = read();
}

void change_to_DFA();

void printDFA();


int main()
{
    int states_num = 0;
    Qs state[states_num];
    int sign_num = 0;
    char signs[sign_num];
    int delta_num = 0;
    char q0 = read();
    int F_num = 0;
    char F[F_num];
    readNFA(states_num, state, sign_num, signs, delta_num, q0, F_num, F);//读取NFA
    vector<set<char>> Q1;
    change_to_DFA();
    printDFA();
    return 0;
}



