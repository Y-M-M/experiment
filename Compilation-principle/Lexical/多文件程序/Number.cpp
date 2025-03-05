#include "Number.h"

Number::Number(const vector<char> &buffer, bool is_number)
{
    number_word.assign(buffer.begin(), buffer.end());
    this->is_number = is_number;
    line = line_num;
}

ostream &operator<<(ostream &Out, const Number &the_number)
{
    all_num++;
    Out << the_number.line << " <";
    if (the_number.is_number)
    {
        Out << "NUMBER," << the_number.number_word;
        NUMBER_num++;
    }
    else
    {
        Out << "ERROR," << the_number.number_word;
        ERROR_num++;
    }
    Out << ">" << endl;
    return Out;
}