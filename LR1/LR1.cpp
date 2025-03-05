#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

typedef struct content // 有效项目的右侧内容
{
    vector<char> readed; // 归约串
    vector<char> waitread; // 待约串(倒序)
    set<char> lookforward; // 向前看符号
    bool operator==(const content& other) const
    {
        if (readed.size() != other.readed.size())
            return false;
        if (waitread.size() != waitread.size())
            return false;
        if (readed.size())
        {
            for (int i = 0; i < readed.size(); i++)
            {
                if (readed[i] != other.readed[i])
                    return false;
            }
        }
        if (waitread.size())
        {
            for (int i = 0; i < waitread.size(); i++)
            {
                if (waitread[i] != other.waitread[i])
                    return false;
            }
        }
        return true;
    }

    bool operator<(const content& other) const {
        // 根据需求定义比较规则，例如按 id 排序
        if (readed.size() >= other.readed.size())
            return false;
        if (waitread.size() >= other.waitread.size())
            return false;
        if (readed.size())
        {
            for (int i = 0; i < readed.size(); i++)
            {
                if (readed[i] >= other.readed[i])
                    return false;
            }
        }
        if (waitread.size())
        {
            for (int i = 0; i < waitread.size(); i++)
            {
                if (waitread[i] >= other.waitread[i])
                    return false;
            }
        }
        return true;
    }
}Content;

class Table
{
public:
    Table(); // 构造函数
    ~Table(); // 析构函数
    friend ostream& operator<< (ostream& out, Table& table); // 打印Table的信息
    void buildfirstset(); // 构造first集合
    void findfirst(const int& pos); // 构造notend[i]的first集合
    void buildfollowset(); // 构造follow集合
    bool findfollow(const char& now, const char& next); // 寻找非终结符now的follow集合
    int whichnotend(const char& c); // 检查是不是非终结符，返回其在非终结符表中的位置(如果不是，则返回表的长度)
    void passit(const char& c, const int& pos); // follow集合的传递
    void builditemset(); // 构造项目集同时构建goto表
    void addcontent(const char& left, map<char, vector<Content>>& state, const Content& content); // 为项目集插入一个content并构建关于这个content的闭包
    bool isequalstate(map<char, vector<Content>>& statea, map<char, vector<Content>>& stateb);
    void printstate(map<char, vector<Content>>& state);
    void createnewcontent(Content& acontent, const int& cnt, const int& leftpos, const int& contentpos);
    void buildRtable(); 
    void readstring();
    void analyzestring();
    void printcontent(Content& content);
    void cirupdate(const char& newleft, const int& i, const set<char>& newforward, map<char, vector<Content>>& state);

private:
    vector<char> endsig; // 终结符号
    vector<char> notend; // 非终结符号
    set<char> reachempty; // 空产生式
    map<char, vector<vector<char> > > generator; // 产生式表
    map<char, set<char> > firstset; // first集合
    map<char, set<char> > followset; // follow集合
    vector<bool> buildedfirst; // 记录该符号是否已经构造过first集合
    vector<bool> buildedfollow; // 记录该符号是否已经构造过follow集合
    char start; // 文法的起始符号
    map<char, vector<char>> son; // 记录follow集合的依赖关系
    vector<bool> havepassfollow; // 记录此依赖关系是否已经传递过follow集合，避免重复传递
    vector<map<char, vector<Content>>> states; 
    // notend存储所有状态；每个状态有多个左部符号；每个左部符号映射至多个产生式右侧
    map<int, map<char, int>> jumptable; // 状态-跳转符号-下一状态
    map<int, map<char, int>> Rtable; // 归约表 状态-跳转符号-产生式编号
    map<int, pair<char, vector<char>>> idtogen; // 将产生式id转换为generator
    map<pair<char, vector<char>>, int> gentoid; // 将generator转换为id
    vector<int> statestack;
    vector<char> sigstack;
    vector<char> inputstack;
    map<pair<char, int>, vector<pair<char, int>>> fa; // 在构建状态时建立的forward集合之间的依赖关系 当左侧更新时也要更新右侧
    // 左侧符号-content的编号
};

Table::Table() // 构造函数，将产生式存入
{
    this->start = 'S'; // 将起始符换成S
    this->notend = {'S', 'E', 'T', 'F'};
    this->endsig = {'+', '-', '*', '/', '(', ')', 'n', '$'}; // n代表num， e代表empty
    vector<char> temp;
    // S的产生式
    temp = {'E'};
    this->generator['S'].push_back(temp);
    idtogen[0] =  make_pair('S', temp);
    gentoid[make_pair('S', temp)] = 0;

    // E的产生式
    temp = {'E', '+', 'T'};
    this->generator['E'].push_back(temp);
    idtogen[1] =  make_pair('E', temp);
    gentoid[make_pair('E', temp)] = 1;

    temp = {'E', '-', 'T'};
    this->generator['E'].push_back(temp);
    idtogen[2] =  make_pair('E', temp);
    gentoid[make_pair('E', temp)] = 2;

    temp = {'T'};
    this->generator['E'].push_back(temp);
    idtogen[3] =  make_pair('E', temp);
    gentoid[make_pair('E', temp)] = 3;

    // T的产生式
    temp = {'T', '*', 'F'};
    this->generator['T'].push_back(temp);
    idtogen[4] =  make_pair('T', temp);
    gentoid[make_pair('T', temp)] = 4;

    temp = {'T', '/', 'F'};
    this->generator['T'].push_back(temp);
    idtogen[5] =  make_pair('T', temp);
    gentoid[make_pair('T', temp)] = 5;

    temp = {'F'};
    this->generator['T'].push_back(temp);
    idtogen[6] =  make_pair('T', temp);
    gentoid[make_pair('T', temp)] = 6;

    // F的产生式
    temp = {'(', 'E', ')'};
    this->generator['F'].push_back(temp);
    idtogen[7] =  make_pair('F', temp);
    gentoid[make_pair('F', temp)] = 7;

    temp = {'n'};
    this->generator['F'].push_back(temp);
    idtogen[8] =  make_pair('F', temp);
    gentoid[make_pair('F', temp)] = 8;
    
    this->buildedfirst.resize(this->notend.size(), false);
    this->buildedfollow.resize(this->notend.size(), false);
    this->havepassfollow.resize(this->notend.size(), false);
}

Table::~Table()
{
}

ostream& operator<< (ostream& out, Table& table)
{
    out << "start:" << endl;
    out << table.start << endl;
    out << endl;
    out << "endsig:" << endl;
    for (int i = 0; i < table.endsig.size(); i++)
    {
        out << table.endsig[i] << ' ';
    }
    out << endl << endl;
    
    out << "notend:" << endl;
    for (int i = 0; i < table.notend.size(); i++)
    {
        out << table.notend[i] << ' ';
    }
    out << endl << endl;

    out << "Generator:" << endl;
    for (int i = 0; i < table.notend.size(); i++)
    {
        out << endl;
        char left = table.notend[i];
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
    for (int i = 0; i < table.notend.size(); i++)
    {
        char state = table.notend[i];
        out << state << ": { ";
        for (char firstchar: table.firstset[state])
        {
            out << firstchar << ' ';
        }
        out << "}" << endl;
    }
    out << endl;

    out << "Followset:" << endl;
    for (int i = 0; i < table.notend.size(); i++)
    {
        char state = table.notend[i];
        out << state << ": { ";
        for (char followchar: table.followset[state])
        {
            out << followchar << ' ';
        }
        out << "}" << endl;
    }
    out << endl;

    out << "Itemset:" << endl;
    for (int i = 0; i < table.states.size(); i++)
    {
        out << endl;
        // out << "i = " << i << endl;
        for (int j = 0; j < table.notend.size(); j++)
        { 
            if (!j) out << i << ":";
            for (Content content: table.states[i][table.notend[j]])
            {
                out << '\t';
                out << table.notend[j] << " -> ";
                if (content.readed.size())
                {
                    for (int k = 0; k < content.readed.size(); k++)
                    {
                        out << content.readed[k];
                    }
                }
                out << '!' ;
                if (content.waitread.size())
                {    
                    for (int k = content.waitread.size() - 1; k >= 0; k--)
                    {
                        out << content.waitread[k];
                    }
                }
                if ((content.waitread.size() + content.readed.size() + 5) / 4 == 1)
                    out << '\t';
                out << '\t';
                if (content.lookforward.size())
                {
                    for (char c: content.lookforward)
                    {
                        out << c << ' ';
                    }
                }
                out << endl;
            }
        }
    }
    out << endl;

    out << "Jumptable:" << endl;
    out << '\t';
    for (int i = 0; i < table.endsig.size(); i++)
    {
        out << table.endsig[i] << '\t';
    }
    for (int i = 0; i < table.notend.size(); i++)
    {
        out << table.notend[i] << '\t';
    }
    out << endl;
    for (int i = 0; i < table.states.size(); i++)
    {
        out << i << '\t';
        for (int j = 0; j < table.endsig.size(); j++)
        {
            if (table.jumptable[i].find(table.endsig[j]) != table.jumptable[i].end())
                out << table.jumptable[i][table.endsig[j]];
            out << '\t';
        }
        for (int j = 0; j < table.notend.size(); j++)
        {
            if (table.jumptable[i].find(table.notend[j]) != table.jumptable[i].end())
                out << table.jumptable[i][table.notend[j]];
            out << '\t';
        }
        out << endl;
    }
    out << endl;

    out << "Rtable:" << endl;
    out << '\t';
    for (int i = 0; i < table.endsig.size(); i++)
    {
        out << table.endsig[i] << '\t';
    }
    for (int i = 0; i < table.notend.size(); i++)
    {
        out << table.notend[i] << '\t';
    }
    out << endl;
    for (int i = 0; i < table.states.size(); i++)
    {
        out << i << '\t';
        for (int j = 0; j < table.endsig.size(); j++)
        {
            if (table.Rtable[i].find(table.endsig[j]) != table.Rtable[i].end())
                out << table.Rtable[i][table.endsig[j]];
            out << '\t';
        }
        for (int j = 0; j < table.notend.size(); j++)
        {
            // if (table.jumptable[i].find(table.notend[j]) != table.jumptable[i].end())
            //     out << table.jumptable[i][table.notend[j]];
            out << '\t';
        }
        out << endl;
    }
    out << endl;

    return out;
}

void Table::buildfirstset()
{
    for (int i = 0; i < this->notend.size(); i++)
    {
        // cout << "i = " << i << endl;
        if (!this->buildedfirst[i])
        {
            // cout << "before" << endl;
            findfirst(i);
            // cout << "after" << endl;
        }
        // cout << "i = " << i << endl;
    }
}

void Table::findfirst(const int &pos) // 为state[pos]构造first集合
{
    char left = this->notend[pos];
    // cout << "left = " << left << ", this->generator[left].size() = " << this->generator[left].size() << endl;
    for (int i = 0; i < this->generator[left].size(); i++) // 遍历其产生式
    {
        // cout << "in findfirst i = " << i << endl;
        int beginpos = 0;
        char firstcharacter = this->generator[left][i][beginpos];
        
        do
        {
            int j = whichnotend(firstcharacter);
            if (j == this->notend.size()) // 是终结符
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
        // cout << "before end i = " << i << endl;
    }
    this->buildedfirst[pos] = true;
}

void Table::buildfollowset()
{
    this->followset[this->start].insert('$'); // 将$符加入文法起始符的follow集合
    for (int i = 0; i < this->notend.size(); i++) // 遍历所有非终结符，构造其显式的follow集合
    {
        char left = this->notend[i];
        // cout << "left = " << left << endl;
        for (int j = 0; j < this->generator[left].size(); j++)
        {
            if (this->generator[left][j].size() == 1) // 产生式右边长度为1
            {
                char firstchar = this->generator[left][j][0];
                if (whichnotend(firstchar) != this->notend.size()) // 唯一的字符是非终结符
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
                    if (this->whichnotend(now) != this->notend.size()) // now是非终结符
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
    for (int i = 0; i < notend.size(); i++)
    {
        this->passit(this->notend[i], i);
    }
    // 删除follow集合中的空产生式
    for (int i = 0; i < notend.size(); i++) 
    {
        if (this->followset[notend[i]].find('e') != this->followset[notend[i]].end())
        {
            this->followset[notend[i]].erase('e');
        }
    }
}

int Table::whichnotend(const char& c)
{
    for (int i = 0; i < this->notend.size(); i++)
    {
        if (c == this->notend[i])
            return i;
    }
    return this->notend.size();
}

bool Table::findfollow(const char& now, const char& next)
{
    // cout << "now = " << now << ", next = " << next << endl;
    if (this->whichnotend(next) != this->notend.size()) // next是非终结符
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
    if (!this->havepassfollow[pos])
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
            passit(ason, whichnotend(ason)); // 将孩子节点集成完毕
            this->followset[c].insert(this->followset[ason].begin(), this->followset[ason].end());
        }
        for (int i = 0; i < needupdate.size(); i++) // 处理相互依赖的关系
        {
            this->followset[needupdate[i]].insert(this->followset[c].begin(), this->followset[c].end());
        }
        this->havepassfollow[pos] = true;
    }
}

void Table::builditemset()
{
    // cout << "builditemset" << endl;
    map<char, vector<Content>> state; // 构建一个项目集
    Content acontent;
    // 构建起点
    for (int i = this->generator[start][0].size() - 1; i >= 0; i--)
        acontent.waitread.push_back(this->generator[start][0][i]); 
    acontent.lookforward.insert('$');
    addcontent(start, state, acontent);
    this->states.push_back(state); // 创建状态0
    int cnt = 0;
    while (cnt < this->states.size())
    {
        this->fa.clear();
        // cout << endl;
        // cout << "cnt = " << cnt << endl;
        state.clear();
        acontent.readed.clear();
        acontent.waitread.clear();
        acontent.lookforward.clear();

        for (int i = 0; i < this->notend.size(); i++) // 跳转符号
        {
            bool needtoadd = false;
            char needfirst = notend[i]; // 跳转符号
            for (int j = 0; j < this->notend.size(); j++) // 左侧符号
            {
                if (this->states[cnt][notend[j]].size() > 0)
                {
                    for (int k = 0; k < this->states[cnt][notend[j]].size(); k++)
                    {
                        if (states[cnt][notend[j]][k].waitread.size() && states[cnt][notend[j]][k].waitread.back() == needfirst)
                        {
                            // cout << "states[cnt][notend[j]][k].waitread.back() = " << states[cnt][notend[j]][k].waitread.back() << ", needfirst = " << needfirst << endl;
                            needtoadd = true;
                            // 构造acontent
                            createnewcontent(acontent, cnt, j, k);
                            addcontent(notend[j], state, acontent);
                            acontent.readed.clear();
                            acontent.waitread.clear();
                            acontent.lookforward.clear();
                        }
                    }
                }
            }
            for (int t = 0; t < states.size(); t++)
            {
                if (this->isequalstate(state, states[t]))
                {
                    // cout << "find it!" << endl;
                    needtoadd = false;  
                    jumptable[cnt][notend[i]] = t;
                }
            }
            if (needtoadd)
            {

                states.push_back(state);
                jumptable[cnt][notend[i]] = states.size() - 1;
            }
            state.clear();
        }
        for (int i = 0; i < this->endsig.size(); i++) // 跳转符号(终结符)
        {
            char needfirst = endsig[i];
            bool needtoadd = false;
            for (int j = 0; j < this->notend.size(); j++) // 左侧符号
            {
                if (this->states[cnt][notend[j]].size() > 0)
                {
                    for (int k = 0; k < this->states[cnt][notend[j]].size(); k++)
                    {
                        if (states[cnt][notend[j]][k].waitread.size() && states[cnt][notend[j]][k].waitread.back() == needfirst)
                        {
                            // cout << "states[cnt][notend[j]][k].waitread.back() = " << states[cnt][notend[j]][k].waitread.back() << ", needfirst = " << needfirst << endl;
                            needtoadd = true;
                            // 构造acontent
                            createnewcontent(acontent, cnt, j, k);
                            // cout << "cnt = " << cnt << ", addcontent" << endl;
                            // cout << "notend[j] = " << notend[j] << endl;
                            addcontent(notend[j], state, acontent);
                            acontent.readed.clear();
                            acontent.waitread.clear();
                            acontent.lookforward.clear();
                        }
                    }
                }
            }
            for (int t = 0; t < states.size(); t++)
            {
                if (this->isequalstate(state, states[t]))
                {    
                    // cout << "findequal!" << endl;
                    needtoadd = false;  
                    jumptable[cnt][endsig[i]] = t;
                }
            }
            if (needtoadd)
            {
                states.push_back(state);
                jumptable[cnt][endsig[i]] = states.size() - 1;
            }
            state.clear();
        }
        cnt++;
    }
}

void Table::addcontent(const char& left, map<char, vector<Content>>& state, const Content& content)
{
    state[left].push_back(content);
    if (!content.waitread.size()) 
        return; // 没有任何衍生项目
    // 构造新的forward
    set<char> newforward;
    char newleft = content.waitread.back();
    // cout << "开始构造forward" << endl;
    if (content.waitread.size() == 1)
    {
        newforward.insert(content.lookforward.begin(), content.lookforward.end()); // 继承向前看符号
    }
    else
    {
        int n = content.waitread.size();
        // 将firstset作为向前看符号
        if (whichnotend(content.waitread[n - 2]) == this->notend.size())
            newforward.insert(content.waitread[n - 2]);
        else
        {
            newforward.insert(this->firstset[content.waitread[n - 2]].begin(), this->firstset[content.waitread[n - 2]].end());
        }
    }
    
    if (whichnotend(newleft) != this->notend.size()) // 非终结符
    {
        vector<vector<char>> rights = this->generator[newleft];
        for (int j = 0; j < rights.size(); j++)
        {
            Content acontent;
            acontent.lookforward = newforward;
            for (int k = rights[j].size() - 1; k >= 0; k--)
            {
                acontent.waitread.push_back(rights[j][k]);
            }
            // cout << newleft << " -> ";
            // printcontent(acontent);
            if (find(state[newleft].begin(), state[newleft].end(), acontent) == state[newleft].end())
            {
                if (content.waitread.size() == 1) // 构建forward集合之间的依赖关系
                    fa[make_pair(left, state[left].size() - 1)].push_back(make_pair(newleft, state[newleft].size())); 
                addcontent(newleft, state, acontent);
            }
            else
            {
                for (int i = 0; i < state[newleft].size(); i++)
                {
                    if (state[newleft][i] == acontent)
                    {
                        // cout << "left = " << left << ", acontent.waitread[0] = " << acontent.waitread[0] << endl;
                        if (content.waitread.size() == 1) // 构建forward集合之间的依赖关系
                            fa[make_pair(left, state[left].size() - 1)].push_back(make_pair(newleft, i)); 
                        cirupdate(newleft, i, newforward, state);
                    }
                }
            }
        }
    } 
}

bool Table::isequalstate(map<char, vector<Content>>& statea, map<char, vector<Content>>& stateb)
{
    for (int i = 0; i < this->notend.size(); i++)
    {
        if (statea[notend[i]].size() != stateb[notend[i]].size())
            return false;
        if (statea[notend[i]].size())
        {
            for (int j = 0; j < statea[notend[i]].size(); j++)
            {
                Content contenta = statea[notend[i]][j];
                Content contentb = stateb[notend[i]][j];
                if (contenta.readed.size() != contentb.readed.size())
                    return false;
                if (contenta.readed.size())
                {
                    for (int k = 0; k < contenta.readed.size(); k++)
                    {
                        if (contenta.readed[k] != contentb.readed[k])
                            return false;
                    }
                }
                if (contenta.waitread.size() != contentb.waitread.size())
                    return false;
                if (contenta.waitread.size())
                {
                    for (int k = 0; k < contentb.waitread.size(); k++)
                    {
                        if (contentb.waitread[k] != contentb.waitread[k])
                            return false;
                    }
                }
                if (contenta.lookforward != contentb.lookforward)
                    return false;
                // if (statea[notend[i]][j] == stateb[notend[i]][j]);
                // else 
                // {
                //     return false;
                // }
            }
        }
        
    }
    return true;
}

void Table::printstate(map<char, vector<Content>>& state)
{
    for (int i = 0; i < this->notend.size(); i++)
    {
        for (Content content: state[notend[i]])
        {
            cout << notend[i] << " -> ";
            if (content.readed.size())
            {
                for (int j = 0; j < content.readed.size(); j++)
                {
                    cout << content.readed[j];
                }
            }
            cout << '!' ;
            if (content.waitread.size())
            {    
                for (int j = content.waitread.size() - 1; j >= 0; j--)
                {
                    cout << content.waitread[j];
                }
            }
            if ((content.waitread.size() + content.readed.size() + 5) / 4 == 1)
                cout << '\t';
            cout << '\t';
            if (content.lookforward.size())
            {
                for (char c: content.lookforward)
                {
                    cout << c << ' ';
                }
            }
            cout << endl;
        }
    }
}

void Table::createnewcontent(Content& acontent, const int& cnt, const int& leftpos, const int& contentpos)
{
    acontent.readed = states[cnt][notend[leftpos]][contentpos].readed;
    acontent.readed.push_back(states[cnt][notend[leftpos]][contentpos].waitread.back());
    acontent.waitread = states[cnt][notend[leftpos]][contentpos].waitread;
    acontent.waitread.pop_back();
    acontent.lookforward = states[cnt][notend[leftpos]][contentpos].lookforward;
}

void Table::buildRtable()
{
    for (int i = 0; i < states.size(); i++) // 遍历所有状态
    {
        for (int j = 0; j < notend.size(); j++) // 遍历该状态所有产生式左部
        {
            for (int k = 0; k < states[i][notend[j]].size(); k++) // 遍历states[i]中notend[j]为左部的所有产生式
            {
                if (!states[i][notend[j]][k].waitread.size()) // 归约串
                {
                    for (char c: states[i][notend[j]][k].lookforward)
                    {
                        if (jumptable[i].find(c) != jumptable[i].end())
                            cout << "i = " << i << ", c = " << c << "出现移进-归约冲突！" << endl;
                        Rtable[i][c] = gentoid[make_pair(notend[j], states[i][notend[j]][k].readed)];
                    }
                }
            }
        }
    }
}

void Table::readstring()
{
    string input;
    cin >> input;
    this->inputstack.push_back('$');
    for (int i = input.length() - 1; i >= 0; i--)
    {
        this->inputstack.push_back(input[i]);
    }
}

void Table::analyzestring()
{
    this->statestack.push_back(0);
    this->sigstack.push_back('$');
    while (inputstack.size())
    {
        // cout << "state = " << statestack.back() << endl;
        char c = inputstack.back();
        // cout << "c = " << c << endl;
        if (this->jumptable[statestack.back()].find(c) != this->jumptable[statestack.back()].end())
        { // 跳转表中有记录
            // cout << "跳转表中有记录" << endl;
            int stateid = jumptable[statestack.back()][c];
            if (whichnotend(c) == this->notend.size()) // 终结符
                cout << "shift" << endl;
            this->statestack.push_back(stateid);
            this->sigstack.push_back(c);
            this->inputstack.pop_back();
        }
        else 
        {// 跳转表中无记录，尝试归约
            // cout << "跳转表中无记录，尝试归约" << endl;
            // cout << "state = " << statestack.back() << ", c = " << c << endl;
            if (this->Rtable[statestack.back()].find(c) != this->Rtable[statestack.back()].end())
            {
                int generatorid = this->Rtable[statestack.back()][c];
                if (!generatorid && c == '$')
                {
                    cout << "accept" << endl;
                    break;
                }
                else
                {
                    char left = idtogen[generatorid].first;
                    vector<char> right = idtogen[generatorid].second;
                    for (int i = 0; i < right.size(); i++)
                    {
                        this->statestack.pop_back();
                        this->sigstack.pop_back();
                    }
                    this->inputstack.push_back(left);
                    cout << generatorid << endl;
                }
            }
            else
            {
                // cout << "state = " << statestack.back() << ", c = " << c << endl;
                cout << "error" << endl;
                break;
            }
        }
    }
}

void Table::printcontent(Content& content)
{
    for (int i = 0; i < content.readed.size(); i++)
    {
        cout << content.readed[i];
    }
    cout << '!';
    for (int j = content.waitread.size() - 1; j >= 0; j--)
    {
        cout << content.waitread[j];
    }
    cout << '\t';
    for (char c: content.lookforward)
    {
        cout << c << ' ';
    }
    cout << endl;
}

void Table::cirupdate(const char& newleft, const int& i, const set<char>& newforward, map<char, vector<Content>>& state)
{
    state[newleft][i].lookforward.insert(newforward.begin(), newforward.end());
    for (int j = 0; j < fa[make_pair(newleft, i)].size(); j++) // 更新依赖它的content的forward
    {
        char needleft = fa[make_pair(newleft, i)][j].first;
        int contentid = fa[make_pair(newleft, i)][j].second;
        cirupdate(needleft, contentid, newforward, state);
    }
}


int main()
{
    Table table;
    table.buildfirstset();
    table.buildfollowset();
    table.builditemset();
    table.buildRtable();
    // cout << table;
    table.readstring();
    table.analyzestring();
    return 0;
}
