#include "Operator.h"

Operator::Operator(const vector<char> &buffer)
{
    operator_word.assign(buffer.begin(), buffer.end());
    line = line_num;
    is_operator = 0;
}

void Operator::judgeit()
{
    switch (operator_word.size())
    {
    case 1:
        if (operator_word == "+" || operator_word == "-" || operator_word == "*" || operator_word == "/" || operator_word == "%" ||
            operator_word == ">" || operator_word == "<" || operator_word == "!" || operator_word == "&" || operator_word == "|" ||
            operator_word == "^" || operator_word == "~" || operator_word == "=" || operator_word == ".")
        {
            is_operator = 1;
        }
        break;
    case 2:
        if (operator_word == "--" || operator_word == "++" || operator_word == "==" || operator_word == "!=" || operator_word == ">=" ||
            operator_word == "<=" || operator_word == "&&" || operator_word == "||" || operator_word == "<<" || operator_word == ">>" ||
            operator_word == "+=" || operator_word == "-=" || operator_word == "/=" || operator_word == "*=" || operator_word == "%=" ||
            operator_word == "&=" || operator_word == "|=" || operator_word == "^=" || operator_word == "->")
        {
            is_operator = 1;
        }
        break;
    case 3:
        if (operator_word == "<<=" || operator_word == ">>=")
        {
            is_operator = 1;
        }
        break;
    }
}

ostream &operator<<(ostream &Out, const Operator &the_operator)
{
    all_num++;
    Out << the_operator.line << " <";
    if (the_operator.is_operator)
    {
        OPERATOR_num++;
        Out << "OPERATOR,";
    }
    else
    {
        ERROR_num++;
        Out << "ERROR,";
    }
    Out << the_operator.operator_word << ">" << endl;
    return Out;
}
