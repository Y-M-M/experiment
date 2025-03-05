#include "Charcon.h"

Charcon::Charcon(vector<char> &buffer)
{
    read_charcon.assign(buffer.begin(), buffer.end());
    is_charcon = 0;
    line = line_num;
}

void Charcon::judgeit()
{
    if (read_charcon[read_charcon.size() - 1] == '\'')
    {
        is_charcon = 1;
        CHARCON_num++;
    }
    else
    {
        is_charcon = 0;
        ERROR_num++;
    }
    all_num++;
}

ostream &operator<<(ostream &Out, Charcon &a_charcon)
{
    Out << a_charcon.line << " <";
    if (a_charcon.is_charcon)
    {
        Out << "CHARCON,";
    }
    else
        cout << "ERROR,";
    Out << a_charcon.read_charcon << ">" << endl;
    return Out;
}
