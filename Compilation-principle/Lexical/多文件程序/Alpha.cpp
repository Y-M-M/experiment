#include "Alpha.h"

Alpha::Alpha(const vector<char> &buffer)
{
    read_alpha.assign(buffer.begin(), buffer.end());
    is_keyword = 0;
    is_identifier = 0;
    line = line_num;
}

bool Alpha::is_alpha(const char &c) const
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool Alpha::is_num(const char &c) const
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

void Alpha::judgeit()
{
    switch (read_alpha[0])
    {
    case 'a':
        if (read_alpha == "auto")
            is_keyword = 1;
        break;
    case 'b':
        if (read_alpha == "break")
            is_keyword = 1;
        break;
    case 'c':
        if (read_alpha == "case" || read_alpha == "char" || read_alpha == "const" || read_alpha == "continue")
            is_keyword = 1;
        break;
    case 'd':
        if (read_alpha == "double" || read_alpha == "default" || read_alpha == "do")
            is_keyword = 1;
        break;
    case 'e':
        if (read_alpha == "else" || read_alpha == "enum" || read_alpha == "extern")
            is_keyword = 1;
        break;
    case 'f':
        if (read_alpha == "float" || read_alpha == "for")
            is_keyword = 1;
        break;
    case 'g':
        if (read_alpha == "goto")
            is_keyword = 1;
        break;
    case 'i':
        if (read_alpha == "int" || read_alpha == "if")
            is_keyword = 1;
        break;
    case 'l':
        if (read_alpha == "long")
            is_keyword = 1;
        break;
    case 'r':
        if (read_alpha == "register" || read_alpha == "return")
            is_keyword = 1;
        break;
    case 's':
        if (read_alpha == "struct" || read_alpha == "sizeof" || read_alpha == "static" || read_alpha == "short" || read_alpha == "signed" || read_alpha == "switch")
            is_keyword = 1;
        break;
    case 't':
        if (read_alpha == "tapered" || read_alpha == "typedef")
            is_keyword = 1;
        break;
    case 'u':
        if (read_alpha == "union" || read_alpha == "unsigned")
            is_keyword = 1;
        break;
    case 'v':
        if (read_alpha == "void" || read_alpha == "volatile")
            is_keyword = 1;
        break;
    case 'w':
        if (read_alpha == "while")
            is_keyword = 1;
        break;
    default:
        break;
    }
    if (!is_keyword)
    {
        int i = 0;
        for (i = 0; i < read_alpha.size(); i++)
        {
            if (is_alpha(read_alpha[i]) || is_num(read_alpha[i]) || (read_alpha[i] == '_'))
                continue;
            else
                break;
        }
        if (i == read_alpha.size())
            is_identifier = 1;
    }
}

ostream &operator<<(ostream &Out, const Alpha &output)
{
    all_num++;
    Out << output.line << " <";
    if (output.is_keyword)
    {
        Out << "KEYWORD";
        KEYWORD_num++;
    }
    else if (output.is_identifier)
    {
        Out << "IDENTIFIER";
        IDENTIFIER_num++;
    }
    else
    {
        Out << "ERROR";
        ERROR_num++;
    }
    Out << "," << output.read_alpha << '>' << endl;
    return Out;
}