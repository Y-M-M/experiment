#include "String.h"

String::String(vector<char> &buffer)
{
    line = line_num;
    read_string.assign(buffer.begin(), buffer.end());
    is_string = 0;
}

void String::judgeit()
{
    if (read_string[read_string.size() - 1] == '\"')
    {
        is_string = 1;
        STRING_num++;
    }
    else
    {
        is_string = 0;
        ERROR_num++;
    }
    all_num++;
}

ostream &operator<<(ostream &Out, String &a_string)
{
    Out << a_string.line << " <";
    if (a_string.is_string)
    {
        Out << "STRING,";
    }
    else
        Out << "ERROR,";
    Out << a_string.read_string << ">" << endl;
    return Out;
}
