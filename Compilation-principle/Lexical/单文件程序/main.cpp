// 目前只支持测评单文件程序，所以需要将代码都写到这个文件中。

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int line_num = 1;
int KEYWORD_num = 0;
int IDENTIFIER_num = 0;
int OPERATOR_num = 0;
int DELIMITER_num = 0;
int CHARCON_num = 0;
int STRING_num = 0;
int NUMBER_num = 0;
int all_num = 0;
int ERROR_num = 0;

int is_num(const char &c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

int is_alpha(const char &c)
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

// 定义一个以字母或下划线开头的类来存储单词，判断这个单词是不是KEYWORD和IDENTIFIER
class Alpha
{
public:
    Alpha(const vector<char> &now_signal);                         // 将识别到的vector转换成Alpa类
    friend ostream &operator<<(ostream &Out, const Alpha &output); // 输出二元组
    void solve_al();                                               // 判断是keyword还是identifier、是否合法
    int is_keyword() const;                                        // 判断是不是keyword
    int is_identifier() const;                                     // 判断是不是identifier

private:
    string signal;  // 字符串(词)
    int line;       // 标记目前的行数
    int keyword;    // 是不是keyword
    int identifier; // 是不是identifier
};

// 将识别到的vector转换成Alpa类
Alpha::Alpha(const vector<char> &now_signal)
{
    signal.assign(now_signal.begin(), now_signal.end());
    line = line_num;
    keyword = 0;
    identifier = 0;
}

// 输出二元组
ostream &operator<<(ostream &Out, const Alpha &output)
{
    Out << output.line << " <";
    if (output.keyword)
        Out << "KEYWORD";
    else if (output.identifier)
        Out << "IDENTIFIER";
    else
        Out << "ERROR";
    Out << "," << output.signal << '>' << endl;
    return Out;
}

// 判断是不是keyword
int Alpha::is_keyword() const
{
    switch (signal[0])
    {
    case 'a':
        if (signal == "auto")
            return 1;
        else
            return 0;
        break;
    case 'b':
        if (signal == "break")
            return 1;
        else
            return 0;
        break;
    case 'c':
        if (signal == "case" || signal == "char" || signal == "const" || signal == "continue")
            return 1;
        else
            return 0;
        break;
    case 'd':
        if (signal == "double" || signal == "default" || signal == "do")
            return 1;
        else
            return 0;
        break;
    case 'e':
        if (signal == "else" || signal == "enum" || signal == "extern")
            return 1;
        else
            return 0;
        break;
    case 'f':
        if (signal == "float" || signal == "for")
            return 1;
        else
            return 0;
        break;
    case 'g':
        if (signal == "goto")
            return 1;
        else
            return 0;
        break;
    case 'i':
        if (signal == "int" || signal == "if")
            return 1;
        else
            return 0;
        break;
    case 'l':
        if (signal == "long")
            return 1;
        else
            return 0;
        break;
    case 'r':
        if (signal == "register" || signal == "return")
            return 1;
        else
            return 0;
        break;
    case 's':
        if (signal == "struct" || signal == "sizeof" || signal == "static" || signal == "short" || signal == "signed" || signal == "switch")
            return 1;
        else
            return 0;
        break;
    case 't':
        if (signal == "tapered" || signal == "typedef")
            return 1;
        else
            return 0;
        break;
    case 'u':
        if (signal == "union" || signal == "unsigned")
            return 1;
        else
            return 0;
        break;
    case 'v':
        if (signal == "void" || signal == "volatile")
            return 1;
        else
            return 0;
        break;
    case 'w':
        if (signal == "while")
            return 1;
        else
            return 0;
        break;

    default:
        break;
    }
    return 0;
}

// 判断是不是identifier
int Alpha::is_identifier() const
{
    for (int i = 0; i < signal.size(); i++)
    {
        if (is_alpha(signal[i]) || is_num(signal[i]) || (signal[i] == '_'))
            continue;
        else
        {
            return 0;
        }
    }
    return 1;
}

// 判断是keyword还是identifier、是否合法
void Alpha::solve_al()
{
    if (is_keyword())
    {
        keyword = 1;
        KEYWORD_num++;
        cout << *this;
        return;
    }
    else if (is_identifier())
    {
        identifier = 1;
        IDENTIFIER_num++;
        cout << *this;
        return;
    }
    else
    {
        ERROR_num++;
        cout << *this;
        return;
    }
}

int is_signal_operator(const char &c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '!' ||
        c == '>' || c == '<' || c == '&' || c == '|' || c == '^' || c == '~' || c == '.')
    {
        return 1;
    }
    else
        return 0;
}

int is_delimter(const char &c)
{
    if (c == ';' || c == ',' || c == ':' || c == '?' || c == '(' || c == ')' || c == '[' ||
        c == ']' || c == '{' || c == '}')
    {
        return 1;
    }
    else
        return 0;
}

void print_delimter(const char &c)
{
    cout << line_num << " <" << "DELIMITER," << c << '>' << endl;
}

// 定义一个类来存储运算符
class Operator
{
public:
    Operator(const vector<char> &now_signal);
    friend ostream &operator<<(ostream &Out, const Operator &the_operator);
    void solve_it();
    int is_operator() const;

private:
    string signal;
    int line;
    int opera;
};

Operator::Operator(const vector<char> &now_signal)
{
    signal.assign(now_signal.begin(), now_signal.end());
    line = line_num;
    opera = 0;
}

ostream &operator<<(ostream &Out, const Operator &the_operator)
{
    Out << the_operator.line << " <";
    if (the_operator.opera)
    {
        Out << "OPERATOR,";
    }
    else
    {
        Out << "ERROR,";
    }
    Out << the_operator.signal << ">" << endl;
    return Out;
}

int Operator::is_operator() const
{
    switch (signal.size())
    {
    case 1:
        if (signal == "+" || signal == "-" || signal == "*" || signal == "/" || signal == "%" ||
            signal == ">" || signal == "<" || signal == "!" || signal == "&" || signal == "|" ||
            signal == "^" || signal == "~" || signal == "=" || signal == ".")
            return 1;
        else
            return 0;
        break;
    case 2:
        if (signal == "--" || signal == "++" || signal == "==" || signal == "!=" || signal == ">=" ||
            signal == "<=" || signal == "&&" || signal == "||" || signal == "<<" || signal == ">>" ||
            signal == "+=" || signal == "-=" || signal == "/=" || signal == "*=" || signal == "%=" ||
            signal == "&=" || signal == "|=" || signal == "^=" || signal == "->")
            return 1;
        else
            return 0;
        break;
    case 3:
        if (signal == "<<=" || signal == ">>=")
            return 1;
        else
            return 0;
        break;
    default:
        return 0;
    }
}

void Operator::solve_it()
{
    opera = is_operator();
    if (opera)
    {
        OPERATOR_num++;
    }
    else
    {
        ERROR_num++;
    }

    cout << *this;
}

class CHARCON
{
public:
    CHARCON(const vector<char> &now_signal);
    void solve_it();
    int is_charcon() const;
    friend ostream &operator<<(ostream &Out, CHARCON &a_charcon);

private:
    string signal;
    int charcon;
    int line;
};

ostream &operator<<(ostream &Out, CHARCON &a_charcon)
{
    Out << a_charcon.line << " <";
    if (a_charcon.charcon)
        Out << "CHARCON";
    else
        Out << "ERROR";
    Out << ',' << a_charcon.signal << '>' << endl;
    return Out;
}

CHARCON::CHARCON(const vector<char> &now_signal)
{
    signal.assign(now_signal.begin(), now_signal.end());
    charcon = 0;
    line = line_num;
}

int CHARCON::is_charcon() const
{
    // if (signal.size() == 3 && signal[2] == '\'')
    // {
    //     return 1;
    // }
    // else if (signal.size() == 4 && signal[3] == '\'' && signal[1] == '\\')
    // {
    //     return 1;
    // }
    // else
    //     return 0;
    if (signal[signal.size() - 1] != '\'')
    {
        return 0;
    }
    return 1;
}

void CHARCON::solve_it()
{
    charcon = is_charcon();
    if (charcon)
    {
        CHARCON_num++;
    }
    else
    {
        ERROR_num++;
    }
    cout << *this;
}

void print_string(vector<char> &now_signal)
{
    string signal;
    signal.assign(now_signal.begin(), now_signal.end());
    cout << line_num << " <" << "STRING," << signal << '>' << endl;
}

int main(int argc, char *argv[])
{
    vector<char> now_signal;    // 用于存储当前识别到的字符
    string file_name = argv[1]; // 该字符串变量为识别的C语言程序文件名
    ifstream test;
    test.open(file_name, ios::in); // 打开文件

    if (!test)
    {
        cout << "打开文件失败" << endl; // 检验文件是否正常打开
        return 0;
    }

    char c; // 逐个读取字符
    test.get(c);
    while (1)
    {
        // cout << "c = " << c << endl;
        int Is_operator = is_signal_operator(c);
        int Is_delimter = is_delimter(c);
        // cout << "Is_operator = " << Is_operator << ", Is_delimter = " << Is_delimter << endl;
        // 处理将词分隔开的符号(注意.既可以作为运算符又可以作为数字的小数点)
        if (c == '\t' || c == ' ' || c == '\n' || test.eof() || Is_operator || Is_delimter || c == '\'' || c == '\"')
        {
            // cout << "分隔：" << "c = " << c << endl;
            if (now_signal.size() != 0)
            {
                if ((now_signal[0] == 'u' && now_signal[1] == '8' && now_signal.size() == 2) || (now_signal[0] == 'u' && now_signal.size() == 1) || (now_signal[0] == 'U' && now_signal.size() == 1) || (now_signal[0] == 'L' && now_signal.size() == 1))
                {
                    if (c == '\"')
                    {
                        now_signal.push_back(c);
                        test.get(c);
                        while (c != '\"')
                        {
                            if (c == '\n')
                            {
                                break;
                            }
                            if (c == '\\')
                            {
                                now_signal.push_back(c);
                                test.get(c);
                            }
                            now_signal.push_back(c);
                            test.get(c);
                        }
                        if (c == '\"')
                            now_signal.push_back(c);
                        all_num++;
                        STRING_num++;
                        print_string(now_signal);
                        now_signal.clear();
                        if (c == '\n')
                            line_num++;
                        test.get(c);
                        continue;
                    }
                    else if (c == '\'')
                    {
                        now_signal.push_back(c);
                        test.get(c);
                        while (c != '\'')
                        {
                            if (c == '\n')
                            {
                                break;
                            }
                            // if (now_signal[1] == '\\' && now_signal.size() == 4 && now_signal[3] != '\'')
                            // {
                            //     break;
                            // }
                            // else if (now_signal[1] != '\\' && now_signal.size() == 3 && now_signal[2] != '\'')
                            //     break;
                            if (c == '\\') // 忽略掉\'
                            {
                                now_signal.push_back(c);
                                test.get(c);
                            }
                            now_signal.push_back(c);
                            test.get(c);
                        }
                        if (c == '\'')
                            now_signal.push_back(c);
                        CHARCON a_charcon(now_signal); // line-1
                        a_charcon.solve_it();
                        now_signal.clear();
                        if (c == '\n')
                            line_num++;
                        test.get(c);
                        continue;
                    }
                }
                // cout << "c = " << c << endl;
                // cout << "now_signal[0] = " << now_signal[0] << endl;
                all_num++;
                // cout << "now_signal[0] = " << now_signal[0] << endl;
                if (is_alpha(now_signal[0]) || now_signal[0] == '_')
                {

                    // cout << "即将创立Alpha" << endl;
                    Alpha a_alpha(now_signal);
                    a_alpha.solve_al();
                    now_signal.clear();
                }
            }
            if (test.eof())
                break;
            else if (c == '\n')
                line_num++;
            else if (Is_delimter)
            {
                DELIMITER_num++;
                all_num++;
                print_delimter(c);
            }
            else if (Is_operator) // 如果运算符后紧接注释，则不能识别注释，报错
            {
                // cout << "c = " << c << endl;
                all_num++;
                now_signal.push_back(c);
                test.get(c);
                if (now_signal[0] == '.' && is_num(c))
                {
                    // cout << "operator之后识别到number" << endl;
                    continue;
                }
                // cout << "c = " << c << endl;
                while (is_signal_operator(c) && !test.eof()) // 可以完整读取运算符或者注释符号
                {
                    now_signal.push_back(c);
                    test.get(c);
                }

                if (now_signal.size() > 1 && now_signal[0] == '/' && (now_signal[1] == '/' || now_signal[1] == '*'))
                {
                    // cout << "检测到注释" << endl;
                    if (now_signal[1] == '/') // 这种格式的注释
                    {
                        // cout << "开始检测//形式的注释" << endl;
                        now_signal.clear();
                        while (c != '\n' && !test.eof())
                            test.get(c);
                        // cout << "//形式的注释检测完毕" << endl;
                        if (c == '\n')
                        {
                            line_num++;
                            test.get(c);
                            continue;
                        }
                        else if (test.eof())
                        {
                            break;
                        }
                    }
                    else if (now_signal[1] == '*') /*这种格式的注释*/
                    {
                        // cout << "开始检测/*形式的注释" << endl;
                        if (now_signal.size() >= 4 && now_signal[now_signal.size() - 2] == '*' && now_signal[now_signal.size() - 1] == '/')
                        {
                            if (test.eof())
                                break;
                            else
                            {
                                now_signal.clear();
                                continue;
                            }
                        }
                        now_signal.clear();
                        char c1 = c;
                        if (c1 == '\n')
                            line_num++;
                        test.get(c);
                        while (1)
                        {
                            // cout << "c1 = " << c1 << ", c = " << c << endl;
                            if (c == '\n')
                            {
                                line_num++;
                            }
                            if (c1 == '*' && c == '/')
                            {
                                break;
                            }
                            c1 = c;
                            test.get(c);
                        }
                        // cout << "/*形式的注释检测完毕" << endl;
                        test.get(c);
                        if (test.eof())
                            break;
                        else
                            continue;
                    }
                }

                // cout << now_signal[0] << endl;

                Operator the_operator(now_signal);
                the_operator.solve_it();

                // cout << now_signal.back() << endl;
                now_signal.clear();
                continue;
            }
            else if (c == '\'')
            {
                now_signal.push_back(c);
                test.get(c);
                while (c != '\'')
                {
                    if (c == '\n')
                    {
                        break;
                    }
                    // if (now_signal[1] == '\\' && now_signal.size() == 4 && now_signal[3] != '\'')
                    // {
                    //     break;
                    // }
                    // else if (now_signal[1] != '\\' && now_signal.size() == 3 && now_signal[2] != '\'')
                    //     break;
                    if (c == '\\') // 忽略掉\'
                    {
                        now_signal.push_back(c);
                        test.get(c);
                    }
                    now_signal.push_back(c);
                    test.get(c);
                }
                if (c == '\'')
                    now_signal.push_back(c);
                CHARCON a_charcon(now_signal); // line-1
                a_charcon.solve_it();
                now_signal.clear();
                if (c == '\n')
                    line_num++;
            }
            else if (c == '\"') // 处理字符串，单独读取完整的一整个字符串
            {
                now_signal.push_back(c);
                test.get(c);
                while (c != '\"')
                {
                    if (c == '\n')
                    {
                        break;
                    }
                    if (c == '\\')
                    {
                        now_signal.push_back(c);
                        test.get(c);
                    }
                    now_signal.push_back(c);
                    test.get(c);
                }
                if (c == '\"')
                    now_signal.push_back(c);
                all_num++;
                if (now_signal[0] == '\"' && now_signal.back() == '\"')
                {
                    STRING_num++;
                    print_string(now_signal);
                }
                else
                {
                    string output;
                    output.assign(now_signal.begin(), now_signal.end());
                    ERROR_num++;
                    cout << line_num << " <ERROR," << output << ">" << endl;
                }
                now_signal.clear();
                if (c == '\n')
                    line_num++;
            }
        }
        else // 将读取到的不能分隔两个字符的符号组成一个单词
        {
            // cout << "不能分隔两个字符：" << c << endl;
            // cout << "now_signal.size() = " << now_signal.size() << endl;
            if (c == '_' || is_alpha(c))
            {
                now_signal.push_back(c);
                test.get(c);
                while ((is_num(c) || c == '_' || is_alpha(c)) && !test.eof())
                {
                    // cout << "c = " << c << endl;
                    now_signal.push_back(c);
                    test.get(c);
                }
                // cout << "识别到的单词" << now_signal[0] << now_signal[1] << endl;
                continue;
            }
            else if (is_num(c)) // 对数字单独处理(只考虑了无符号数)
            {
                if (c == '0')
                {
                    now_signal.push_back(c);
                    test.get(c);
                    if (c != '.')
                    {
                        if (c == 'x' || c == 'X')
                        {
                            now_signal.push_back(c);
                            test.get(c);
                            while ((is_num(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') && !test.eof()))
                            {
                                now_signal.push_back(c);
                                test.get(c);
                            }
                        }
                        else
                        {
                            while (c >= '0' && c <= '7' && !test.eof())
                            {
                                now_signal.push_back(c);
                                test.get(c);
                            }
                        }
                    }
                }
                else
                {
                    now_signal.push_back(c);
                    test.get(c);
                }

                while (is_num(c) && !test.eof())
                {
                    now_signal.push_back(c);
                    test.get(c);
                }

                if (c == '.')
                {
                    now_signal.push_back(c);
                    test.get(c);
                    while (is_num(c) && !test.eof())
                    {
                        now_signal.push_back(c);
                        test.get(c);
                    }
                }
                int find_error = 0;
                if (c == 'E' || c == 'e')
                {
                    now_signal.push_back(c);
                    test.get(c);
                    find_error = 1;
                    if (c == '-' || c == '+')
                    {
                        now_signal.push_back(c);
                        test.get(c);
                    }
                    while (is_num(c) && !test.eof())
                    {
                        find_error = 0;
                        now_signal.push_back(c);
                        test.get(c);
                    }
                }

                while ((c == 'l' || c == 'L' || c == 'u' || c == 'U') && !test.eof())
                {
                    now_signal.push_back(c);
                    test.get(c);
                }
                if ((c == 'f' || c == 'F') && !test.eof())
                {
                    now_signal.push_back(c);
                    test.get(c);
                }

                if (find_error || (is_alpha(c) && !test.eof()))
                {
                    // cout << "c = " << c << endl;
                    string output;
                    if (is_alpha(c))
                    {
                        now_signal.push_back(c);
                        output.assign(now_signal.begin(), now_signal.end());
                        test.get(c);
                    }
                    else
                    {
                        // cout << "出错" << endl;
                        output.assign(now_signal.begin(), now_signal.end());
                        // cout << "else中output:" << output << endl;
                        // cout << "now_signal[0]:" << now_signal[0] << endl;
                    }
                    ERROR_num++;
                    all_num++;
                    // cout << "output:" << output << endl;
                    cout << line_num << " <ERROR," << output << '>' << endl;
                    now_signal.clear();
                }
                else
                {
                    string output;
                    output.assign(now_signal.begin(), now_signal.end());
                    NUMBER_num++;
                    all_num++;
                    cout << line_num << " <NUMBER,";
                    cout << output << '>' << endl;
                    now_signal.clear();
                }
                continue;
            }
            else
            {
                cout << line_num << " <ERROR," << c << ">" << endl;
                all_num++;
                ERROR_num++;
            }
        }
        test.get(c);
    }

    test.close(); // 关闭文件
    cout << line_num << endl;
    cout << KEYWORD_num << ' ' << IDENTIFIER_num << ' ' << OPERATOR_num << ' ' << DELIMITER_num << ' ' << CHARCON_num << ' ' << STRING_num << ' ' << NUMBER_num << endl;
    cout << ERROR_num;
    return 0;
}