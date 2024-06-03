/*构造与下推自动机等价的上下文无关文法
用户输入一个下推自动机 M = (Q, T, Γ, δ, q0, z0, F)
*/
/*
*/


#include <iostream>
#include <vector>
#include <string>
using namespace std;

//PDA
// 定义 PDA 的转移函数结构体
//eg.δ（q，a，z）=（γ,ε）
//右侧仅允许一个
//左侧相同时不要合并

//转换函数
typedef struct 
{
    char state;         //当前状态
    char input_symbol;  // 输入符号
    char stack_top;     // 栈顶符号
    char next_state;    // 下一状态
    string next_stack_top;       //转换后栈顶符号
} PDA_transition;

// 定义 PDA 结构体
typedef struct {
    vector<char> states; //状态集合
    vector<char> symbol; //字母表
    vector<char> stacksymbol; //栈字符
    int transitions_num; //转换函数个数
    vector<PDA_transition> transitions;//转换函数
    char initial_state;          // 初态q0
    char stack_start_symbol ;    //下推栈起始符z0
    vector<char> accept_states;       // 终态集合
} PDA;


//CFG

//三元组
typedef struct {
    char state;
    char input_symbol;
    char next_state;
} Production_Symbol;

// 定义产生式结构体
//[q,z,γ]→a或[q,z,qk]→a[γ,B1,q1][q1,B2,q2]…[qk-1,Bk,qk]
typedef struct {
    Production_Symbol left_symbol;   // 产生式左部[q, z, γ]
    char right_start; //a或ε
    Production_Symbol right_symbols[10];  // 产生式右部
} Production;

// 定义上下文无关文法结构体
typedef struct {
    vector<char> non_terminals;  // 非终结符集合，假设最多有两个非终结符
    vector<char> terminals;      // 终结符集合，假设最多有两个终结符
    vector<Production> productions; // 产生式集合
    char start_symbol;      // 起始符号
} CFG;


CFG init_CFG(CFG* Pcfg);//初始化CFG
void input_PDA_transition(PDA& Ppda);//用户输入PDA转移函数
PDA inputPDA();//用户输入PDA下推自动机
CFG PDAtoCFG(PDA pda) ;// PDA转化为CFG函数
void print_PDA(PDA& pda);//打印PDA
void print_CFG(CFG cfg);//打印CFG


int main(int argc, const char* argv[]) {

    //读取PDA
    printf("本程序实现由下推自动机构造等价的上下文无关文法，接下来请您输入需要进行转化的PDA：\n");
    PDA pda = inputPDA();
    print_PDA(pda);
    CFG cfg = PDAtoCFG(pda);
    print_CFG(cfg);

    return 0;
}

//用户输入PDA转移函数
void input_PDA_transition(PDA& pda) 
{
    PDA_transition a_transition;
    for(int i = 0; i < pda.transitions_num; i++)
    {
        cin >> a_transition.state >> a_transition.input_symbol >> a_transition.stack_top
            >> a_transition.next_state;
    }
    pda.transitions.push_back(a_transition);
}

//用户输入PDA下推自动机
PDA inputPDA() {
    PDA pda;
    PDA* Ppda = &pda;

    char c;
    printf("请输入PDA的状态集（用空格分隔），以回车结束：\n");
    while(c = getchar() != '\n')
    {
        if(c == ' ');
        else pda.states.push_back(c);
    }

    printf("请输入NFA字母表（用空格分隔），以回车结束：\n");
    while(c = getchar() != '\n')
    {
        if(c == ' ');
        else pda.symbol.push_back(c);
    }

    printf("请输入PDA的栈符号集合（用空格分隔），以回车结束：\n");
    while(c = getchar() != '\n')
    {
        if(c == ' ');
        else pda.stacksymbol.push_back(c);
    }

    printf("请输入您即将输入的转移函数的个数：\n");
    cin >> pda.transitions_num;

    printf("请输入转移函数，格式为p(当前状态) q(输入字符) r(下一个状态) v(输入后栈顶符号)，以回车结束每一个转移函数：\n");
    input_PDA_transition(pda);

    printf("请输入初始状态：\n");
    cin >> pda.initial_state;

    printf("请输入栈起始符：\n");
    cin >> pda.stack_start_symbol;

    printf("请输入终止状态集合，以空格分隔，以回车结束：\n");
    while((c = getchar()) != '\n')
    {
        if(c == ' ');
        else pda.accept_states.push_back(c);
    }

    return pda;
}

//初始化CFG
CFG init_CFG(CFG* Pcfg) {
    // 初始化非终结符集合
    char non_terminals[] = {'S'};
    int num_non_terminals = sizeof(non_terminals) / sizeof(non_terminals[0]);
    // 将初始化后的数据赋值给输入的 Pcfg
    for (int i = 0; i < num_non_terminals; i++) {
        Pcfg->non_terminals[i] = non_terminals[i];
    }
    //初始化终结符集合
    Pcfg->terminals[0] = 0;



    return *Pcfg;
}

void print_PDA(PDA& pda) 
{
    printf("状态集合：\n");
    for(int i = 0; i < pda.states.size(); i++) 
    {
        printf("%c ",pda.states[i]);
    }
    printf("\n");
    printf("字母表集合：\n");
    for(int i = 0; i < pda.symbol.size(); i++) {
        printf("%c ",pda.symbol[i]);
    }
    printf("\n");
    printf("栈符号集合T：\n");
    for(int i = 0; i < pda.stacksymbol.size(); i++) {
        printf("%c ", pda.stacksymbol[i]);
    }
    printf("\n");
    printf("转换函数: \n");
    for(int i = 0; i < pda.transitions_num; i++)
    {
        cout << '(' << pda.transitions[i].input_symbol << ", " 
            << pda.transitions[i].input_symbol << ", "
            << pda.transitions[i].stack_top << ")";
        cout << "->(" << pda.transitions[i].next_state << ", "
            << pda.transitions[i].next_stack_top << ")" << endl;
    }
    printf("起始符q0：\n");
    cout << pda.initial_state;
    printf("栈起始符：\n");
    cout << pda.stack_start_symbol;
    printf("终止符号集: \n");
    for(int i = 0; i < pda.accept_states.size(); i++)
        cout << pda.accept_states[i] << ' ';
    cout << endl;
}

//打印CFG
void print_CFG(CFG cfg) {
    printf("由该PDA构造的CFG如下：\n");
    printf("终结符集合T：\n");
    int i, j;
    int T_length = sizeof(cfg.terminals) / sizeof(cfg.terminals[0]);
    for(i = 0; i < T_length; i++) {
        printf("%c ",cfg.terminals[i]);
    }
    printf("\n");
    printf("产生式集合：\n");
    int production_length = sizeof(cfg.productions) / sizeof(cfg.productions[0]);
    for(i = 0; i < production_length; i++) {
        printf("[%c,%c,%c] -> %c",cfg.productions[i].left_symbol.state,
         cfg.productions[i].left_symbol.input_symbol, cfg.productions[i].left_symbol.next_state, cfg.productions[i].right_start);
        int right_length = sizeof(cfg.productions[0].right_symbols) / sizeof(cfg.productions[0].right_symbols[0]);
        for(j = 0; j < right_length; j++) {
            printf("[%c,%c,%c]",cfg.productions[i].right_symbols[j].state,
             cfg.productions[i].right_symbols[j].input_symbol, cfg.productions[i].right_symbols[j].next_state);
        }
        printf("\n");
    }
}


// PDA转化为CFG函数
CFG PDAtoCFG(PDA pda) {
    CFG cfg;
    //终结符集合T
    strcpy(cfg.terminals, pda.input_symbols);
    //1.对于每个q∈Q，将S→[q0，z0, q] 加入到产生式中。
    int i;
    for(i = 0; i < pda.states_num; i++) {
        printf("S -> [q0, z0, %c]\n", pda.states[i]);
    }
    //2.若δ（q，a，z）含有（γ,ε），则将[q,z,γ]→a加入到产生式中。
    //3.若δ（q，a，z）含有（γ，B1B2…Bk）k≥1，Bi∈Γ，
    //则对Q中的每一个状态序列q1,q2,…,qk,(qi∈Q),把形如[q,z,qk]→a[γ,B1,q1][q1,B2,q2]…[qk-1,Bk,qk]的产生式加入到P中。
    //其中，a  T  或 a = 
    int j = 0;
    for(i = 0; i < pda.transitions_num; i++) {
        if(pda.transitions[i].next_stack_top[0] == 'ε'){//若δ（q，a，z）含有（γ,ε），则将[q,z,γ]→a加入到产生式中
            //产生式左边
            cfg.productions[j].left_symbol.state = pda.transitions[i].state;
            cfg.productions[j].left_symbol.input_symbol = pda.transitions[i].input_symbol;
            cfg.productions[j].left_symbol.next_state = pda.transitions[i].next_state;
            //产生式右边
            cfg.productions[j].right_start = 'ε';
            j++;
        }
        else{//3.若δ（q，a，z）含有（γ，B1B2…Bk）k≥1，Bi∈Γ，
            //则对Q中的每一个状态序列q1,q2,…,qk,(qiQ),把形如[q,z,qk]→a[γ,B1,q1][q1,B2,q2]…[qk-1,Bk,qk]的产生式加入到P中。
            int length = sizeof(cfg.productions[j].right_symbols) / sizeof(cfg.productions[j].right_symbols[0]);
            int Qknum = 1;//Q的k次方
            //产生式左边
            cfg.productions[j].left_symbol.state = pda.transitions[i].state;
            cfg.productions[j].left_symbol.input_symbol = pda.transitions[i].input_symbol;
            cfg.productions[j].left_symbol.next_state = pda.transitions[i].next_state;
            //产生式右边
            cfg.productions[j].right_start = pda.transitions[i].input_symbol;
            for(int num = 0; num < length; num++) {
                Qknum *= length;
            }
            for(int k = 0; k < Qknum; k++) {
                cfg.productions[j].right_symbols[k].state = pda.transitions[i].next_state;
                // cfg.productions[j].right_symbols[k].input_symbol = 
                // cfg.productions[j].right_symbols[k].next_state = 
            }
            j++;
        }

    }

    return cfg;
}
