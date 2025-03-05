#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

typedef struct tran//PDA转换函数
{
    string now_state;//现在的状态
    string input_signal;//输入字符
    string now_stack_top;//现在的栈顶元素
    string next_state;//下一个状态
    string next_stack_top;//下一个栈顶元素

    friend ostream& operator<<(ostream &Ost, struct tran a_transition)//重定向输出
    {
        Ost << '(' << a_transition.now_state << ", " 
            << a_transition.input_signal << ", " 
            << a_transition.now_stack_top << ")->("
            << a_transition.next_state << ", " 
            << a_transition.next_stack_top << ')';
        return Ost;
    }

    bool operator==(const struct tran& other) const//用于比较大小
    {
        return ((now_state == other.now_state) && (input_signal == other.input_signal)
                && (now_stack_top == other.now_stack_top)
                && (next_state == other.next_state)
                && (next_stack_top == other.next_stack_top));
    }
}Tran;

typedef struct new_symbol//文法的非终结符
{
    string now_state;//现在的状态
    string stack_top;//栈顶元素
    string next_state;//下一个状态

    friend ostream& operator<<(ostream &Ost, struct new_symbol a_symbol)//重定向输出
    {
        Ost << '[' << a_symbol.now_state << ", " << a_symbol.stack_top
            << ", " << a_symbol.next_state << ']';
        return Ost;
    }

    bool operator==(const struct new_symbol &other) const//比较是否相等
    {
        return ((now_state == other.now_state)
                && (stack_top == other.stack_top)
                && (next_state == other.next_state));
    }
}New_symbol;

typedef struct pro//文法产生式
{
    New_symbol now_new_signal;//现在的非终结符
    string right_start;//生成式右边的终结符或空串
    vector<New_symbol> next_new_signal;//生成式右边的非终结符串

    friend ostream& operator<<(ostream &Ost, struct pro &a_produce_function)//重定向输出
    {
        Ost <<  a_produce_function.now_new_signal
            << "->" << a_produce_function.right_start;
        int num = a_produce_function.next_new_signal.size();
        for(int i = 0; i < num; i++)
        {
            Ost << a_produce_function.next_new_signal[i];
        }
        return Ost;
    }
    bool operator==(const struct pro& other) const //比较是否相等
    {
        int is_equ = 1;
        if(next_new_signal.size() == other.next_new_signal.size())
        {
            for(int i = 0; i < next_new_signal.size(); i++)
            {
                if(next_new_signal[i] == other.next_new_signal[i]);
                else  is_equ = 0;
            }
        }
        else is_equ = 0;
        return ((now_new_signal == other.now_new_signal)
                && (right_start == other.right_start)
                && is_equ);
    }
}Pro;

typedef struct begin_pro//文法的由起始符开始的产生式
{
    char begin_signal;//文法的起始符
    string right_start;//符号或者空
    vector<New_symbol> next_new_signal;//生成式右边的非终结符

    friend ostream& operator<<(ostream &Ost, struct begin_pro &a_produce_function)//重载输出
    {
        Ost <<  a_produce_function.begin_signal
            << "->" << a_produce_function.right_start;
        int num = a_produce_function.next_new_signal.size();
        for(int i = 0; i < num; i++)
        {
            Ost << a_produce_function.next_new_signal[i];
        }
        return Ost;
    }
}Begin_pro;

class CFG;

class PDA//PDA
{
    public:
        PDA();//构造函数
        ~PDA();//析构函数
        void input_PDA_transtions(const int &trans_num);//输入PDA的转移函数
        CFG generate_cfg() const;//生成cfg
        void change_to_empty();
        friend ostream& operator<<(ostream &Ost, PDA &pda)//重定向输出PDA
        {
            Ost << "状态集如下:" << endl;
            for(int i = 0; i < pda.states_num; i++)
            {
                Ost << pda.states[i] << ' ';
            }
            Ost << endl;
            Ost << "符号表如下:" << endl;
            for(int i = 0; i < pda.symbol_num; i++)
            {
                Ost << pda.symbols[i] << ' ';
            }
            Ost << endl;
            Ost << "栈符号如下:" << endl;
            for(int i = 0; i < pda.stack_sym_num; i++)
            {
                Ost << pda.stack_sym[i] << " ";
            }
            Ost << endl;
            Ost << "转换函数如下:" << endl;
            for(int i = 0; i < pda.trans_num; i++)
            {
                Ost << pda.trans[i];
                Ost << endl;
            }
            Ost << "起始字符如下:" << endl;
            Ost << pda.start_state << endl;
            Ost << "栈起始符号如下:" << endl;
            Ost << pda.stack_bottom << endl;
            Ost << "栈终结状态如下:" << endl;
            for(int i = 0; i < pda.final_state_num; i++)
            {
                Ost << pda.final_state[i] << " ";
            }
            Ost << endl;
            return Ost;
        }
    private:
        int states_num;
        vector<string> states;//PDA的状态数目
        int symbol_num;
        vector<string> symbols;//PDA的输入字符表
        int stack_sym_num;
        vector<string> stack_sym;//PDA的栈字符表
        int trans_num;
        vector<Tran> trans;//PDA的转换函数
        string start_state;//PDA的起始状态
        string stack_bottom;//栈起始符
        int final_state_num;
        vector<string> final_state;//PDA的终结状态
};

class CFG
{
    public:
        CFG();//构造函数
        ~CFG();//析构函数
        void add_end_symbols(string a_symobl);//添加终结符
        void add_begin_productions(Begin_pro& a_begin_pro);//添加由起始符开始的文法产生式
        void add_production(Pro& a_pro);//添加文法产生式
        void add_noend_symbles(New_symbol& a_noend);//添加非终结符
        friend ostream& operator<<(ostream &Ost, CFG cfg)//重定向输出CFG
        {
            Ost << "生成的CFG如下所示:" << endl;
            int num = cfg.noend_symbols.size();
            Ost << "非终结符如下:" << endl;
            for(int i = 0; i < num; i++)
            {
                Ost << cfg.noend_symbols[i] << endl;
            }
            Ost << "终结符如下:" << endl;
            num = cfg.end_symbols.size();
            for(int i = 0; i < num; i++)
            {
                Ost << cfg.end_symbols[i] << ' ';
            }
            Ost << endl;
            Ost << "文法产生式如下:" << endl;
            num = cfg.begin_productions.size();
            for(int i = 0; i < num; i++)
            {
                Ost << cfg.begin_productions[i] << endl;
            }
            num = cfg.productions.size();
            for(int i = 0; i < num; i++)
            {
                Ost << cfg.productions[i] << endl;
            }
            Ost << "文法的起始符如下:" << endl;
            Ost << cfg.start_symbol << endl;
            return Ost;
        }
    private:
        vector<New_symbol> noend_symbols;//文法的非终结符
        vector<string> end_symbols;//文法的终结符
        vector<Pro> productions;//文法的产生式
        vector<Begin_pro> begin_productions;//由起始符开始的文法产生式
        char start_symbol;//起始符
};