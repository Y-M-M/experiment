#include <iostream>
#include <vector>
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
            << a_transition.now_stack_top << ')';
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

int main()
{
    cout << "下面输入的是字符串：" << endl;
    Tran a_transition;
    cin >> a_transition.now_state >> a_transition.input_signal
            >> a_transition.now_stack_top 
            >> a_transition.next_state 
            >> a_transition.next_stack_top;
    cout << "输出读入的数据:" << endl;
    cout << a_transition.now_state << ' ' << a_transition.input_signal
         << ' ' << a_transition.now_stack_top 
         << ' ' << a_transition.next_state << ' '
         << a_transition.now_stack_top << endl;
    return 0;
}