#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

class Table{
public:
    Table(); // 构造函数
    ~Table(); // 析构函数
    friend ostream& operator<< (ostream& out, Table& table); // 打印Table的信息
    void buildfirstset(); // 构造first集合
    void findfirst(const int& pos); // 构造notends[i]的first集合
    void buildfollowset(); // 构造follow集合
    bool findfollow(const char& now, const char& next); // 寻找非终结符now的follow集合
    int is_notend(const char& c); // 检查是不是非终结符，返回其在非终结符表中的位置(如果不是，则返回表的长度)
    void passit(const char& c, const int& pos); // follow集合的传递
    bool buildtable(); // 构造分析表
    void addnull(ostream& out, const int& num); // 统一输出格式
    bool analyzeinput(); // 对输入字符串进行分析
    void read_input(); // 读取用户输入
    int returnid(const char& left, const int& num); // 查找产生式对应的id
    void printanalyzestack(); // 打印符号栈
    void printinputcharstack(); // 打印分析栈
    
private:
    vector<char> characters; // 终结符号
    vector<char> notends; // 非终结符号
    set<char> reachempty; // 空产生式
    map<char, vector<vector<char> > > generator; // 产生式表
    map<char, set<char> > firstset; // first集合
    map<char, set<char> > followset; // follow集合
    vector<char> buildedfirst; // 记录该符号是否已经构造过first集合
    vector<char> buildedfollow; // 记录该符号是否已经构造过follow集合
    char start; // 文法的起始符号
    map<char, vector<char>> son; // 记录follow集合的依赖关系
    vector<bool> passfollow; // 记录此依赖关系是否已经传递过follow集合，避免重复传递
    map <char, map<char, vector<char> > > M; // 分析表
    map <char, map<char, int> > id; // 分析表中产生式对应的id
    vector<char> analyzestack; // 分析栈
    vector<char> inputcharstack; // 符号栈
};

Table::Table() // 构造函数，将产生式存入
{
    this->start = 'E';
    this->notends = {'E', 'T', 'A', 'F', 'B'};
    this->characters = {'+', '-', '*', '/', '(', ')', 'n', 'e', '$'}; // n代表num， e代表empty
    vector<char> temp;
    // E的产生式
    temp = {'T', 'A'};
    this->generator['E'].push_back(temp);
    // A的产生式
    temp = {'+', 'T', 'A'};
    this->generator['A'].push_back(temp);
    temp = {'-', 'T', 'A'};
    this->generator['A'].push_back(temp);
    temp = {'e'};
    this->generator['A'].push_back(temp);
    // T的产生式
    temp = {'F', 'B'};
    this->generator['T'].push_back(temp);
    // B的产生式
    temp = {'*', 'F', 'B'};
    this->generator['B'].push_back(temp);
    temp = {'/', 'F', 'B'};
    this->generator['B'].push_back(temp);
    temp = {'e'};
    this->generator['B'].push_back(temp);
    // F的产生式
    temp = {'(', 'E', ')'};
    this->generator['F'].push_back(temp);
    temp = {'n'};
    this->generator['F'].push_back(temp);
    this->buildedfirst.resize(this->notends.size(), false);
    this->buildedfollow.resize(this->notends.size(), false);
    this->reachempty.insert('A');
    this->reachempty.insert('B');
    this->passfollow.resize(this->notends.size(), false);
    this->analyzestack.push_back('$');
    this->inputcharstack.push_back('$');
    this->analyzestack.push_back(this->start);
}

Table::~Table()
{
}

ostream& operator<< (ostream& out, Table& table)
{
    out << "start:" << endl;
    out << table.start << endl;
    out << endl;
    out << "Characters:" << endl;
    for (int i = 0; i < table.characters.size(); i++)
    {
        out << table.characters[i] << ' ';
    }
    out << endl << endl;
    
    out << "notends:" << endl;
    for (int i = 0; i < table.notends.size(); i++)
    {
        out << table.notends[i] << ' ';
    }
    out << endl << endl;

    out << "Generator:" << endl;
    for (int i = 0; i < table.notends.size(); i++)
    {
        out << endl;
        char left = table.notends[i];
        for (int j = 0; j < table.generator[left].size(); j++)
        {
            out << left << " -> ";
            for (int k = 0; k < table.generator[left][j].size(); k++)
            {
                out << table.generator[left][j][k];
            }
            out << endl;
        }
    }
    out << endl;

    out << "Firstset:" << endl;
    for (int i = 0; i < table.notends.size(); i++)
    {
        char notend = table.notends[i];
        out << notend << ": { ";
        for (char firstchar: table.firstset[notend])
        {
            out << firstchar << ' ';
        }
        out << "}" << endl;
    }
    out << endl;

    out << "Followset:" << endl;
    for (int i = 0; i < table.notends.size(); i++)
    {
        char notend = table.notends[i];
        out << notend << ": { ";
        for (char followchar: table.followset[notend])
        {
            out << followchar << ' ';
        }
        out << "}" << endl;
    }
    out << endl;

    out << "Table:" << endl;
    int width = 8;
    int rows = table.notends.size();
    int cols = table.characters.size();
    for (int i = -1; i < rows; i++)
    { 
        // out << "i = " << i << endl;
        for (int j = -1; j < cols; j++)
        {
            if (table.characters[j] == 'e') continue;
            vector<char> printout;
            if (i == -1 && j != -1)
            {
                printout.push_back(table.characters[j]);
            }
            else if (i != -1 && j == -1)
            {
                printout.push_back(table.notends[i]);
            }
            else
            {
                printout = table.M[table.notends[i]][table.characters[j]];
            }
            for (int k = 0; k < printout.size(); k++)
                out << printout[k];
            table.addnull(out, width - printout.size()); // 对齐
        }
        out << endl;
    }
    out << endl;

    out << "Id:" << endl;
    for (int i = -1; i < rows; i++)
    { 
        // out << "i = " << i << endl;
        for (int j = -1; j < cols; j++)
        {
            if (table.characters[j] == 'e') continue;
            vector<char> printout;
            if (i == -1 && j != -1)
            {
                printout.push_back(table.characters[j]);
            }
            else if (i != -1 && j == -1)
            {
                printout.push_back(table.notends[i]);
            }
            else
            {
                int num = table.id[table.notends[i]][table.characters[j]];
                if (num >= 0 && num <= 9)
                    printout.push_back(num + '0');
                else
                {
                    printout.push_back('1');
                    printout.push_back('0');
                }
                // cout << "pirntout.back() = " << printout.back() << endl;
                // cout << "table.id[table.notends[i]][table.characters[j]] = " << table.id[table.notends[i]][table.characters[j]] << endl;
            }
            for (int k = 0; k < printout.size(); k++)
                out << printout[k];
            table.addnull(out, width - printout.size()); // 对齐
        }
        out << endl;
    }
    out << endl;

    return out;
}

/**
 * @brief 构造非终结符的first集合
 * 遍历notends数组，对每个非终结符调用函数构造first结合，构造成功则改变标识位
 */
void Table::buildfirstset()
{
    for (int i = 0; i < this->notends.size(); i++)
    {
        if (!this->buildedfirst[i])
        {
            findfirst(i);
        }
    }
}

void Table::findfirst(const int &pos) // 为notend[pos]构造first集合
{
    char left = this->notends[pos];
    for (int i = 0; i < this->generator[left].size(); i++) // 遍历其产生式
    {
        int beginpos = 0;
        char firstcharacter = this->generator[left][i][beginpos];
        
        do
        {
            int j = is_notend(firstcharacter);
            if (j == this->notends.size()) // 是终结符
            {
                this->firstset[left].insert(firstcharacter); // 将终结符|e插入到first集合中
                firstcharacter = 0;
            }
            else  // 是非终结符
            {
                if (!this->buildedfirst[j] && j != pos) // 之前没有构造过
                    findfirst(j);
                this->firstset[left].insert(this->firstset[firstcharacter].begin(), this->firstset[firstcharacter].end());
                firstcharacter = 0;
                if (reachempty.find(firstcharacter) != reachempty.end())
                {
                    beginpos++;
                    firstcharacter = this->generator[left][i][beginpos];
                }
            }

        } while (firstcharacter != 0); // 处理产生式右边第一个字符产生空的情况
    }
    this->buildedfirst[pos] = true;
}

void Table::buildfollowset()
{
    this->followset[this->start].insert('$'); // 将$符加入文法起始符的follow集合
    for (int i = 0; i < this->notends.size(); i++) // 遍历所有非终结符，构造其显式的follow集合
    {
        char left = this->notends[i];
        // cout << "left = " << left << endl;
        for (int j = 0; j < this->generator[left].size(); j++)
        {
            if (this->generator[left][j].size() == 1) // 产生式右边长度为1
            {
                char firstchar = this->generator[left][j][0];
                if (is_notend(firstchar) != this->notends.size()) // 唯一的字符是非终结符
                {
                    son[firstchar].push_back(left); // 建立follow集合的继承关系
                    // cout << "build pass relation with " << firstchar << endl;
                }
            }
            else // 产生式右边长度大于1
            {
                int n = this->generator[left][j].size();
                for (int a = 0; a < n; a++) // 遍历产生式右边的所有字符
                {
                    char now = this->generator[left][j][a];
                    // cout << "now = " << now << endl;
                    if (this->is_notend(now) != this->notends.size()) // now是非终结符
                    {
                        if (a == n - 1) // 遍历到最后一个字符
                        {
                            son[now].push_back(left);
                            // cout << "build pass relaion with " << now << endl;
                        }
                        else
                        {
                            char next = this->generator[left][j][a + 1]; 
                            bool result = this->findfollow(now, next);
                            int newnextpos = a + 1;
                            while (!result)
                            {
                                newnextpos++;
                                if (newnextpos == n)
                                {
                                    son[now].push_back(left);
                                    // cout << "build pass relaion with " << now << endl;
                                    break;
                                }
                                else
                                {
                                    result = this->findfollow(now, this->generator[left][j][newnextpos]);
                                }
                            }
                        }
                    }
                    // 如果now是终结符，则无需处理
                }
            }
        }
    }
    // follow集合的传递
    for (int i = 0; i < notends.size(); i++)
    {
        this->passit(this->notends[i], i);
    }
    // 删除follow集合中的空产生式
    for (int i = 0; i < notends.size(); i++) 
    {
        if (this->followset[notends[i]].find('e') != this->followset[notends[i]].end())
        {
            this->followset[notends[i]].erase('e');
        }
    }
}

int Table::is_notend(const char& c)
{
    for (int i = 0; i < this->notends.size(); i++)
    {
        if (c == this->notends[i])
            return i;
    }
    return this->notends.size();
}

bool Table::findfollow(const char& now, const char& next)
{
    // cout << "now = " << now << ", next = " << next << endl;
    if (this->is_notend(next) != this->notends.size()) // next是非终结符
    {
        followset[now].insert(firstset[next].begin(), firstset[next].end());
        // cout << "insert " << next << "'s firstset" << endl;
        if (this->reachempty.find(next) != this->reachempty.end()) // next可产生空产生式
        {
            // cout << next << " can produce empty" << endl;
            return false;
        }
    }
    else // next是终结符
    {
        followset[now].insert(next);
        // cout << "add " << next << "to followser[" << now << "]" << endl;
    }
    return true;
}

void Table::passit(const char& c, const int& pos)
{
    if (!this->passfollow[pos])
    {
        vector<char> needupdate;
        for (int i = 0; i < this->son[c].size(); i++) 
        {
            char ason = son[c][i];
            if (find(this->son[ason].begin(), this->son[ason].end(), c) != this->son[ason].end()) // 如果互为父子
            {
                needupdate.push_back(ason);
                auto newEnd = remove(this->son[ason].begin(), this->son[ason].end(), c);
                son[ason].erase(newEnd, son[ason].end()); // 消除循环依赖
            }
            passit(ason, is_notend(ason)); // 将孩子节点集成完毕
            this->followset[c].insert(this->followset[ason].begin(), this->followset[ason].end());
        }
        for (int i = 0; i < needupdate.size(); i++) // 处理相互依赖的关系
        {
            this->followset[needupdate[i]].insert(this->followset[c].begin(), this->followset[c].end());
        }
        this->passfollow[pos] = true;
    }
}

bool Table::buildtable()
{
    for (int i = 0; i < this->notends.size(); i++) // 遍历所有非终结符
    {
        char notend = this->notends[i];
        for (int j = 0; j < this->generator[notend].size(); j++) // 遍历非终结符notend的所有产生式
        {
            int nowid = returnid(notend, j); // 获得给定目前的产生式id
            // cout << "nowid = " << nowid << endl;
            vector<char> newchars = this->generator[notend][j]; // 产生式右部
            // 考虑first
            int firstpos = 0; 
            if (this->is_notend(newchars[firstpos]) != this->notends.size())  // 非终结符
            {
                do
                {
                    for (char c: this->firstset[newchars[firstpos]]) // 遍历该终结符的firstset的所有成员
                    {
                        if (this->M[notend][c].size()) return false; // 出现冲突，不是LL(1)文法
                        M[notend][c] = newchars; // 填表
                        id[notend][c] = nowid;
                        // cout << "notend = " << notend << ", c = " << c << ", nowid = " << nowid << "id[notend][c] = " << id[notend][c] << endl;
                    } 
                    if (this->reachempty.find(newchars[firstpos]) != this->reachempty.end()) // 当前字符可能产生空
                    {
                        firstpos++;
                        if (firstpos == newchars.size()) // 产生式右部可以为空串
                        {
                            // 考虑follow集合
                            for (char c: this->followset[notend])
                            {
                                if (this->M[notend][c].size()) return false; // 出现冲突，不是LL(1)文法
                                M[notend][c] = newchars;
                                id[notend][c] = nowid;
                                // cout << "notend = " << notend << ", c = " << c << ", nowid = " << nowid << "id[notend][c] = " << id[notend][c] << endl;
                            }
                        }
                    }
                    else
                    {
                        firstpos = -1;
                    }
                } while (firstpos != -1 && firstpos < newchars.size());
            }
            else // 终结符 特别考虑空串
            {
                if (this->M[notend][newchars[firstpos]].size()) return false; // 出现冲突，不是LL(1)文法
                M[notend][newchars[firstpos]] = newchars;
                id[notend][newchars[firstpos]] = nowid;
                // cout << "notend = " << notend << ", c = " << newchars[firstpos] << ", nowid = " << nowid << ", id[notend][c] = " << id[notend][newchars[firstpos]] << endl;
                if (newchars.size() == 1 && newchars[0] == 'e')
                {
                    // 考虑follow集合
                    for (char c: this->followset[notend])
                    {
                        if (this->M[notend][c].size()) return false; // 出现冲突，不是LL(1)文法
                        M[notend][c] = newchars;
                        id[notend][c] = nowid;
                        // cout << "notend = " << notend << ", c = " << c << ", nowid = " << nowid << "id[notend][c] = " << id[notend][c] << endl;
                    }
                }
            }
        }
    }
    return true;
}

void Table::addnull(ostream& out, const int& num)
{
    for (int i = 0; i < num; i++)
    {
        out << ' ';
    }
}

bool Table::analyzeinput()
{
    read_input();
    char i = analyzestack.back();
    char j = inputcharstack.back();
    while (i != '$' || j != '$')
    {
        printanalyzestack();
        printinputcharstack();
        
        if (is_notend(i) == this->notends.size()) // 终结符
        {
            if (i == j)
            {
                this->analyzestack.pop_back();
                this->inputcharstack.pop_back();
                cout << "match" << endl;
            }
            else
            {
                cout << "error" << endl;
                return false;
            }
        }
        else // 非终结符
        {
            vector<char> newchars;
            // 执行动作
            newchars = M[i][j];
            if (!newchars.size())
            {
                cout << "error" << endl;
                return false;
            }
            else
            {
                cout << id[i][j] << endl;
                if (newchars.size() == 1 && newchars[0] == 'e')
                {
                    this->analyzestack.pop_back();
                }
                else
                {
                    this->analyzestack.pop_back();
                    for (int cnt = newchars.size() - 1; cnt >= 0; cnt--)
                    {
                        this->analyzestack.push_back(newchars[cnt]);
                    }
                }
            }
        }
        i = analyzestack.back();
        j = inputcharstack.back();
    }
    printanalyzestack();
    printinputcharstack();
    cout << "accept" << endl;
    return true;
}

void Table::read_input() // 用户输入存入符号栈
{
    string input;
    cin >> input;
    for (int i = input.length() - 1; i >= 0; i--)
    {
        this->inputcharstack.push_back(input[i]);
    }
}

int Table::returnid(const char& left, const int& num)
{
    if (left == 'E')
        return 1;
    else if (left == 'A')
    {
        switch (num)
        {
            case 0:
                return 2;
            case 1:
                return 3;
            case 2:
                return 4;
        }
    }
    else if (left == 'T')
        return 5;
    else if (left == 'B')
    {
        switch (num)
        {
            case 0:
                return 6;
            case 1:
                return 7;
            case 2:
                return 8;
        }
    }
    else if (left == 'F')
    {
        switch (num)
        {
        case 0:
            return 9;
        case 1:
            return 10;
        }
    }
    return 0;
}

void Table::printanalyzestack()
{
    for (int i = 0; i < this->analyzestack.size(); i++)
    {
        cout << this->analyzestack[i];
    }
    cout << '\t';
}

void Table::printinputcharstack()
{
    for (int i = this->inputcharstack.size() - 1; i >= 0; i--)
    {
        cout << this->inputcharstack[i];
    }
    cout << '\t';
}

int main()
{
    Table table;
    table.buildfirstset();
    table.buildfollowset();
    if (!table.buildtable())
    {
        cout << "该文法不是LL(1)文法！" << endl;
    }
    else
    {
        table.analyzeinput();
    }
    // cout << table;
    return 0;
}