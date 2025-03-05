#include "term.h"

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

Read_word::Read_word(const string &filename) // 构造函数，打开文件
{
    this->file.open(filename, ios::in); // 打开文件
    if (!file)                          // 检验文件是否成功打开
    {
        cout << "打开文件失败" << endl;
    }
}

bool Read_word::reach_end() const // 判断是否到文档尾
{
    return file.eof();
}

bool Read_word::Is_signal_operator() const // 判断是不是operator
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '!' ||
        c == '>' || c == '<' || c == '&' || c == '|' || c == '^' || c == '~' || c == '.')
    {
        return 1;
    }
    else
        return 0;
}

bool Read_word::Is_delimter() const // 判断是不是delimter
{
    if (c == ';' || c == ',' || c == ':' || c == '?' || c == '(' || c == ')' || c == '[' ||
        c == ']' || c == '{' || c == '}')
    {
        return 1;
    }
    else
        return 0;
}

bool Read_word::Is_word_end() const // 判断当前符号能不能隔开两个单词
{
    if (c == '\t' || c == ' ' || c == '\n' || Is_signal_operator() || Is_delimter() || (c == '\'') || c == '\"')
    {
        return 1;
    }
    else
        return 0;
}

bool Read_word::is_modify_string() const
{
    if ((buffer[0] == 'u' && buffer[1] == '8' && buffer.size() == 2) || (buffer[0] == 'u' && buffer.size() == 1) || (buffer[0] == 'U' && buffer.size() == 1) || buffer[0] == 'L' && buffer.size() == 1)
        return 1;
    else
        return 0;
}

void Read_word::read_string() // 阅读字符串word，不会判断是否出错
{
    buffer.push_back(c);
    file.get(c);
    while (c != '\"')
    {
        if (c == '\n') // 字符串不能跨行
        {
            break;
        }
        if (c == '\\') // 忽略转义字符
        {
            buffer.push_back(c);
            file.get(c);
        }
        buffer.push_back(c);
        file.get(c);
    }
    if (c == '\"') // 正常退出，以"结尾，没出错
    {
        buffer.push_back(c);
    }
    else if (c == '\n')
    {
        line_num++;
    }
}

void Read_word::read_charcon() // 读入charword
{
    buffer.push_back(c);
    file.get(c);
    while (c != '\'')
    {
        if (c == '\n') // 未结束即遇到空格，出错
            break;
        else if (c == '\\') // 忽略转义字符
        {
            buffer.push_back(c);
            file.get(c);
        }
        buffer.push_back(c);
        file.get(c);
    }
    if (c == '\'')
    {
        buffer.push_back(c);
    }
    else if (c == '\n')
    {
        line_num++;
    }
}

bool Read_word::is_alpha(const char &c) const
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

bool Read_word::is_num(const char &c) const
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

void Read_word::skipit()
{
    if (buffer[1] == '/') // 这种格式的注释
    {
        buffer.clear();
        while (c != '\n' && !file.eof())
            file.get(c);
        if (c == '\n')
            line_num++;
    }
    else if (buffer[1] == '*') /*这种格式的注释*/
    {
        if (buffer.size() >= 4 && buffer[buffer.size() - 2] == '*' && buffer[buffer.size() - 1] == '/')
            buffer.clear();
        else
        {
            buffer.clear();
            char c1 = c;
            if (c1 == '\n')
                line_num++;
            file.get(c);
            while (!(c1 == '*' && c == '/') && !file.eof())
            {
                if (c == '\n')
                    line_num++;
                c1 = c;
                file.get(c);
            }
        }
    }
}

bool Read_word::read_num()
{
    if (c != '0')
    {
        buffer.push_back(c);
        file.get(c);
    }
    else
    {
        buffer.push_back(c);
        file.get(c);
        if (c == '.')
            ; // 小数
        else  // 识别八进制和十六进制
        {
            if (c == 'x' || c == 'X')
            {
                buffer.push_back(c);
                file.get(c);
                while ((is_num(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') && !file.eof())) // 识别十六进制
                {
                    buffer.push_back(c);
                    file.get(c);
                }
            }
            else
            {
                while (c >= '0' && c <= '7' && !file.eof()) // 识别八进制
                {
                    buffer.push_back(c);
                    file.get(c);
                }
            }
        }
    }

    // 识别十进制
    while (is_num(c) && !file.eof())
    {
        buffer.push_back(c);
        file.get(c);
    }

    if (c == '.')
    {
        buffer.push_back(c);
        file.get(c);
        while (is_num(c) && !file.eof())
        {
            buffer.push_back(c);
            file.get(c);
        }
    }

    int find_error = 0;
    if (c == 'E' || c == 'e')
    {
        buffer.push_back(c);
        file.get(c);
        find_error = 1;
        if (c == '-' || c == '+')
        {
            buffer.push_back(c);
            file.get(c);
        }
        if (is_num(c))
            find_error = 0;
        while (is_num(c) && !file.eof())
        {
            buffer.push_back(c);
            file.get(c);
        }
    }

    while ((c == 'l' || c == 'L' || c == 'u' || c == 'U') && !file.eof())
    {
        buffer.push_back(c);
        file.get(c);
    }

    if ((c == 'f' || c == 'F') && !file.eof())
    {
        buffer.push_back(c);
        file.get(c);
    }
    if (find_error || (is_alpha(c) && !file.eof()))
        return 0;
    else
        return 1;
}

void Read_word::process_buffer() // 处理当前buffer中存储的单词
{
    if (buffer.size() == 0)
        return;
    else
    {
        if (is_modify_string()) // 判断当前符号是不是修饰字符串或单个字符的字符
        {
            if (c == '\"') // 读取并识别识别u8或u或U或L修饰的字符串
            {
                read_string();
                String string_word(buffer);
                string_word.judgeit();
                cout << string_word;
                buffer.clear();
            }
            else if (c == '\'') // 读取并识别u8或u或U或L修饰的单个字符
            {
                read_charcon();
                Charcon charcon_word(buffer);
                charcon_word.judgeit();
                cout << charcon_word;
                buffer.clear();
            }
            file.get(c);
        } // 如果不是特殊修饰的字符串，还可以继续判断是不是变量名或关键词
        if (buffer.size() > 0 && is_alpha(buffer[0]) || buffer[0] == '_') // 以字母或下划线开头
        {
            Alpha alpha_word(buffer);
            alpha_word.judgeit();
            cout << alpha_word;
            buffer.clear();
        }
    }
}

void Read_word::solve_it() // 读取独立的单词
{
    file.get(c);
    while (!file.eof())
    {
        if (Is_word_end()) // 上一个读取到的单词可以结束了
        {
            if (buffer.size() != 0) // 处理缓冲区中已经存在的单词
            {
                process_buffer();
            }
            if ((!file.eof()) && (c == ' ' || c == '\t'))
            {
                file.get(c);
            }
            else if ((!file.eof()) && c == '\n')
            {
                line_num++;
                file.get(c);
            }
            else if ((!file.eof()) && Is_delimter())
            {
                Delimter delimter_word(c);
                cout << delimter_word;
                file.get(c);
            }
            else if ((!file.eof()) && Is_signal_operator())
            {
                buffer.push_back(c);
                file.get(c);
                if (buffer[0] == '.' && is_num(c)) // 数字
                    continue;
                while (Is_signal_operator() && !file.eof()) // 完整读取运算符或者注释符号
                {
                    buffer.push_back(c);
                    file.get(c);
                }
                if (buffer.size() > 1 && (buffer[0] == '/' && buffer[1] == '/') || (buffer[0] == '/' && buffer[1] == '*'))
                {
                    skipit();
                    if (!file.eof())
                        file.get(c);
                }
                else
                {
                    Operator operator_word(buffer);
                    operator_word.judgeit();
                    cout << operator_word;
                    buffer.clear();
                }
            }
            else if ((!file.eof()) && c == '\'')
            {
                read_charcon();
                Charcon charcon_word(buffer);
                charcon_word.judgeit();
                cout << charcon_word;
                buffer.clear();
                file.get(c);
            }
            else if ((!file.eof()) && c == '\"')
            {
                read_string();
                String string_word(buffer);
                string_word.judgeit();
                cout << string_word;
                buffer.clear();
                file.get(c);
            }
        }
        else // 当读取到不能分隔两个字符的符号时
        {
            if (c == '_' || is_alpha(c)) // 读取keyword或delimter
            {
                buffer.push_back(c);
                file.get(c);
                while ((is_num(c) || c == '_' || is_alpha(c)) && !file.eof())
                {
                    buffer.push_back(c);
                    file.get(c);
                }
            }
            else if (is_num(c))
            {
                bool is_number = read_num();
                Number number_word(buffer, is_number);
                cout << number_word;
                buffer.clear();
            }
            else
            {
                cout << line_num << " <ERROR," << c << ">" << endl;
                all_num++;
                ERROR_num++;
                file.get(c);
            }
        }
    }
    if (buffer.size() != 0) // 处理最后一个词
    {
        process_buffer();
    }

    cout << line_num << endl;
    cout << KEYWORD_num << ' ' << IDENTIFIER_num << ' ' << OPERATOR_num << ' ' << DELIMITER_num << ' ' << CHARCON_num << ' ' << STRING_num << ' ' << NUMBER_num << endl;
    cout << ERROR_num;
}

int main(int argc, char *argv[])
{
    string filename = argv[1]; // 本程序识别的C语言程序的文件名
    Read_word buffer(filename);
    buffer.solve_it();
    return 0;
}