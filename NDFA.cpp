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

typedef struct transfer_new
{
    char sign;
    set<char> next;
}Transfer_new;

typedef struct newQ{
    set<char> stateset;
    vector<Transfer_new> trans;
}NewQ;

char read()//忽略空格和回车，读取字符
{
    char c = getchar();
    while(c == ' ' || c == '\n') c = getchar();
    return c;
}


void intial(vector<NewQ> new_Q, char &q0, Qs *state)
{
    set <char> DFA_state;
    DFA_state.insert(q0);
    int position = 0;
    new_Q[++position].stateset = DFA_state;
}

void find_state()
{

}

void printNFA(int &states_num, Qs * states, int &sign_num, char* signs, char &q0, int &F_num, char *F)
{
    printf("请确认您输入的NFA：\n");
    printf("Q:\n");
    for(int i = 0; i < states_num; i++)
    {
        printf("%c%c", states[i].state, " \n"[i == states_num - 1]);
    }
    printf("T:\n");
    for(int i = 0; i < sign_num; i++)
    {
        printf("%c%c", signs[i], " \n"[i == sign_num - 1]);
    }
    printf("delta:\n");
    for(int i = 0; i < states_num; i++)
    {
        int p = states[i].state;
        int my_size = states[i].trans.size();
        for(int j = 0; j < my_size; j++)
        {
            int q = states[i].trans[j].sign;
            int r = states[i].trans[j].next;
            printf("%c %c %c\n", p, q, r);
        }
    }
    printf("q0:\n");
    printf("%c\n", q0);
    printf("F:\n");
    for(int i = 0; i < F_num; i++)
    {
        printf("%c%c", F[i], " \n"[i == F_num-1]);
    }
}

void printDFA(){

}


int main()
{
    //读取NFA
    printf("本程序完成从NFA到DFA的转化，接下来请您输入需要进行转化的NFA：\n");
    //读取Q
    printf("请输入NFA状态的个数：\n");
    int states_num = 0;
    scanf("%d", &states_num);
    Qs states[states_num];
    printf("请依次输入状态名称，以空格或回车分隔不同状态(请保证状态名称是单个字符)：\n");
    for(int i = 0; i < states_num; i++)  states[i].state = read();
    //读取T
    printf("请输入NFA字母表的个数：\n");
    int sign_num = 0;
    scanf("%d", &sign_num);
    char signs[sign_num];
    printf("请依次输入NFA包含的字符，以空格或回车分隔不同的状态：\n");
    for(int i = 0; i < sign_num; i++) signs[i] = read();
    //读取δ
    printf("请输入您将输入的转移函数的个数：\n");
    int delta_num = 0;
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
            if(states[j].state == p)  states[j].trans.push_back({q, r});
        }
    }
    //读取q0
    printf("请输入起始状态q0：\n");
    char q0 = read();
    q0 = read();
    //读取T
    printf("请输入终结状态T的数目：\n");
    int F_num = 0;
    char F[F_num];
    scanf("%d", &F_num);
    printf("请依次输入终结状态：\n");
    for(int i = 0; i < F_num; i++) F[i] = read();
    
    
    printNFA(states_num, states, sign_num, signs, q0, F_num, F);
    vector<NewQ> new_Q;
    intial(new_Q, q0, states);//将起始状态初始化
    find_state();
    printDFA();
    return 0;
}



