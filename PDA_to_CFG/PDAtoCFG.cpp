#include "PDAtoCFG.h"

PDA::PDA()//构造函数，用户输入PDA
{
    cout << "本程序实现由下推自动机构造等价的上下文无关文法，接下来请您输入需要进行转化的PDA: " << endl;
    cout << "请输入PDA的状态数目：" << endl;
    cin >> states_num;
    string name;
    cout << "请输入PDA的状态集（用空格分隔），以回车结束（如果您输入的是终态接受的PDA，请不要将qe和ql作为状态名称）：" << endl;
    for(int i = 0; i < states_num; i++)
    {
        cin >> name;
        states.push_back(name);
    }
    cout << "请输入PDA字符的个数" << endl;
    cin >> symbol_num;
    cout << "请输入PDA字母表（用空格分隔），请确保您输入的字符不会互为前缀，以回车结束：（如果您输入的是终态接受的PDA，请不要将z1作为栈字符）" << endl;
    for(int i = 0; i < symbol_num; i++)
    {
        cin >> name;
        symbols.push_back(name);
    }
    cout << "请输入PDA的栈符号的个数：" << endl;
    cin >> stack_sym_num;
    cout << "请输入PDA的栈符号集合（用空格分隔），以回车结束：" << endl;
    for(int i = 0; i < stack_sym_num; i++)
    {
        cin >> name;
        stack_sym.push_back(name);
    }
    cout << "请输入您即将输入的转移函数的个数:" << endl;
    cin >> trans_num;
    cout << "请输入转移函数，格式为p(当前状态) q(输入字符) u(当前栈顶符号) r(下一个状态) v(输入后栈顶符号)，以回车结束每一个转移函数，请用[empty]表示空字符：" << endl;
    for(int i = 0; i < trans_num; i++)
    {
        input_PDA_transtions(trans_num);
    }
    cout << "请输入初始状态：" << endl;
    cin >> start_state;
    cout << "请输入栈起始符：" << endl;
    cin >> stack_bottom;
    cout << "请输入终止状态数目:" << endl;
    cin >> final_state_num;
    cout << "请输入终止状态集合，以空格分隔，以回车结束：" << endl;
    for(int i = 0; i < final_state_num; i++)
    {
        cin >> name;
        final_state.push_back(name);
    }
    if(final_state_num)
        change_to_empty();
}

PDA::~PDA(){};

void PDA::input_PDA_transtions(const int &trans_num)
{
    Tran a_transition;
    cin >> a_transition.now_state >> a_transition.input_signal
            >> a_transition.now_stack_top 
            >> a_transition.next_state 
            >> a_transition.next_stack_top;
    // cout << "输出读入的transition:" << endl;
    // cout << a_transition << endl;
    trans.push_back(a_transition);
}

CFG PDA::generate_cfg() const
{
    CFG cfg;
    int num;
    num = this->symbol_num;
    for(int i = 0; i < num; i++)//添加终结符
    {
        cfg.add_end_symbols(this->symbols[i]);
    }
    num = this->states_num;
    for(int i = 0; i < num; i++)//添加由起始符开始的产生式
    {
        Begin_pro a_begin_production;
        a_begin_production.begin_signal = 'S';
        a_begin_production.right_start = "";
        New_symbol a_new_symbol;
        a_new_symbol.now_state = this->start_state;
        a_new_symbol.stack_top = this->stack_bottom;
        a_new_symbol.next_state = states[i];
        a_begin_production.next_new_signal.push_back(a_new_symbol);
        cfg.add_begin_productions(a_begin_production);
    }
    num = this->trans_num;
    for(int i = 0; i < num; i++)//遍历转移函数，构造文法生成式
    {
        if(trans[i].next_stack_top == "[empty]")//处理next_stack_top为空的情况
        {
            New_symbol a_new_symbol;
            a_new_symbol.now_state = trans[i].now_state;
            a_new_symbol.stack_top = trans[i].now_stack_top;
            a_new_symbol.next_state = trans[i].next_state;
            Pro a_pro;
            a_pro.now_new_signal = a_new_symbol;
            a_pro.right_start = trans[i].input_signal;
            cfg.add_production(a_pro);
            cfg.add_noend_symbles(a_new_symbol);
        }
        else
        {
            vector<string> seperate;//将下一状态的下推栈顶分离成单个的下推栈字符
            // cout << "ok1" << endl;
            for(int j = 0; j < trans[i].next_stack_top.size(); j++)//对next_stack_top的每一个字符查看有没有匹配的栈符号
            {
                // cout << "ok2" << endl;
                for(int k = 0; k < stack_sym_num; k++)//遍历每一个栈符号，看是否和目前位置的next_stack_top的字符匹配
                {   
                    // cout << "ok3" << endl;
                    auto jt = trans[i].next_stack_top.begin() + j;
                    auto it = find(trans[i].next_stack_top.begin(), trans[i].next_stack_top.end(), stack_sym[k][0]);
                    
                    while(it != trans[i].next_stack_top.end())
                    {
                        //cout << "ok5" << endl;
                        if(it == jt) 
                        {
                            int ok = 1;
                            for(int t = 0; t < stack_sym[k].size(); t++)
                            {
                                if(it[t] != jt[t]) ok = 0;
                            }
                            if(ok) seperate.push_back(stack_sym[k]);
                            break;
                        }
                        it = find(it + 1, trans[i].next_stack_top.end(), stack_sym[k][0]);
                    }
                    // cout << "ok4" << endl;
                }
            }
            int length = seperate.size();
            // cout << "输出分离前的字符串" << endl;
            // cout << trans[i].next_stack_top << endl;
            // cout << "输出分离好的字符串" << endl;
            // for(int j = 0; j < length; j++)
            // {
            //     cout << seperate[j] << endl;
            // }
            if(length == 1)
            {
                
                for(int j = 0; j < states_num; j++)
                {
                    New_symbol new_symbol1;
                    New_symbol new_symbol2;
                    Pro a_production;
                    a_production.right_start = trans[i].input_signal;
                    new_symbol1.now_state = trans[i].now_state;
                    new_symbol1.stack_top = trans[i].now_stack_top;
                    new_symbol1.next_state = states[j];
                    cfg.add_noend_symbles(new_symbol1);
                    a_production.now_new_signal = new_symbol1;
                    new_symbol2.now_state = trans[i].next_state;
                    new_symbol2.stack_top = trans[i].next_stack_top;
                    new_symbol2.next_state = states[j];
                    cfg.add_noend_symbles(new_symbol2);
                    a_production.next_new_signal.push_back(new_symbol2);
                    a_production.right_start = trans[i].input_signal;
                    cfg.add_production(a_production);
                }
            }
            else if(length == 2)//额外压入一个栈顶字符
            {
                for(int j = 0; j < states_num; j++)
                {
                    for(int k = 0; k < states_num; k++)
                    {
                        New_symbol new_symbol1;
                        New_symbol new_symbol2;
                        New_symbol new_symbol3;
                        Pro a_production;
                        new_symbol1.now_state = trans[i].now_state;
                        new_symbol1.stack_top = trans[i].now_stack_top;
                        new_symbol1.next_state = states[j];//左边的字符遍历所有下一个状态的情况
                        cfg.add_noend_symbles(new_symbol1);//将左边的字符添加到new_symobl1中

                        a_production.now_new_signal = new_symbol1;
                        a_production.right_start = trans[i].input_signal;

                        new_symbol2.now_state = trans[i].next_state;
                        new_symbol3.next_state = states[j];
                        new_symbol2.stack_top = seperate[0];
                        new_symbol2.next_state = states[k];
                        new_symbol3.now_state = states[k];
                        new_symbol3.stack_top = seperate[1];

                        a_production.next_new_signal.push_back(new_symbol2);
                        a_production.next_new_signal.push_back(new_symbol3);

                        cfg.add_production(a_production);
                    }
                }
            }
            else if(length == 3)//额外压入两个栈顶字符
            {
                for(int j = 0; j < states_num; j++)
                {
                    for(int k = 0; k < states_num; k++)
                    {
                        for(int t = 0; t < states_num; t++)
                        {
                            New_symbol new_symbol1;
                            New_symbol new_symbol2;
                            New_symbol new_symbol3;
                            New_symbol new_symbol4;
                            Pro a_production;

                            new_symbol1.now_state = trans[i].now_state;
                            new_symbol1.next_state = states[j];
                            new_symbol1.stack_top = trans[i].now_stack_top;

                            new_symbol2.now_state = trans[i].next_state;
                            new_symbol2.next_state = states[k];
                            new_symbol2.stack_top = seperate[0];

                            new_symbol3.now_state = states[k];
                            new_symbol3.next_state = states[t];
                            new_symbol3.stack_top = seperate[1];

                            new_symbol4.now_state = states[t];
                            new_symbol4.next_state = states[j];
                            new_symbol4.stack_top = seperate[2];

                            cfg.add_noend_symbles(new_symbol1);
                            cfg.add_noend_symbles(new_symbol2);
                            cfg.add_noend_symbles(new_symbol3);
                            cfg.add_noend_symbles(new_symbol4);

                            a_production.now_new_signal = new_symbol1;
                            a_production.right_start = trans[i].input_signal;
                            a_production.next_new_signal.push_back(new_symbol2);
                            a_production.next_new_signal.push_back(new_symbol3);
                            a_production.next_new_signal.push_back(new_symbol4);

                            cfg.add_production(a_production);
                        }
                    }
                }
            }
        }
    }
    return cfg;
}

void PDA::change_to_empty()
{
    string new_state;
    //添加状态
    new_state = "qe";
    states.push_back(new_state);
    new_state = "ql";
    states.push_back(new_state);
    //修改起始状态和栈起始符
    string before_start = start_state;
    start_state = "ql";
    stack_bottom = "z1";
    
    //修改转换函数
    Tran a_tran;
    a_tran.now_state = start_state;
    a_tran.input_signal = "[empty]";
    a_tran.now_stack_top = stack_bottom;
    a_tran.next_state = before_start;
    a_tran.next_stack_top = stack_bottom + before_start;
    trans.push_back(a_tran);
    //将终态转换至空栈
    for(int i = 0; i < final_state_num; i++)
    {
        for(int j = 0; j < stack_sym_num; j++)
        {
            Tran other_tran;
            other_tran.now_state = final_state[i];
            other_tran.now_stack_top = stack_sym[j];
            other_tran.input_signal = "[empty]";
            other_tran.next_state = "qe";
            other_tran.next_stack_top = "[empty]";
            trans.push_back(other_tran);
        }
    }
    for(int i = 0; i < stack_sym_num; i++)
    {
        Tran next_tran;
        next_tran.now_state = "qe";
        next_tran.now_stack_top = stack_sym[i];
        next_tran.input_signal = "[empty]";
        next_tran.next_stack_top = "[empty]";
        next_tran.next_state = "qe";
    }
    //改变终态
    final_state_num = 0;
    final_state.clear();
}

CFG::CFG()
{
    start_symbol = 'S';
};


CFG::~CFG(){};

void CFG::add_end_symbols(string a_symbol)
{
    end_symbols.push_back(a_symbol);
}

void CFG::add_begin_productions(Begin_pro& a_begin_pro)
{
    begin_productions.push_back(a_begin_pro);
}

void CFG::add_production(Pro& a_pro)
{
    if(find(productions.begin(), productions.end(), a_pro) == productions.end())
        this->productions.push_back(a_pro);
}

void CFG::add_noend_symbles(New_symbol& a_noend)
{
    if(find(noend_symbols.begin(), noend_symbols.end(), a_noend) == noend_symbols.end())
        this->noend_symbols.push_back(a_noend);
}
