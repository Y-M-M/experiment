// 线性时间选择 一分为三的方法
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <ctime>
#include <random>
using namespace std;

class K_dist
{
public:
    K_dist();
    bool ascending(const int &l, const int &r) const;  // 判断是不是升序序列
    bool descending(const int &l, const int &r) const; // 判断是不是降序序列
    int find_it(const int &position);                  // 处理对不同排名的基站的寻找
    int RandomizedSelect(const int &l, const int &r);  // 线性时间选择
    void exchange_it(const int &a, const int &b);      // 交换两个数及基站名
    int check_nums() const;                            // 查询总的基站数目
    void printit(const int &position) const;           // 打印基站的名字和k_dist
    void print_dists() const;                          // 打印当前all_dist的状态
    void print_names() const;                          // 打印当前names的状态

private:
    vector<double> all_dists; // 存储需要排序的数字
    vector<string> names;     // 存储基站的名字
    vector<int> selected;     // 记录已经被用作过基准的dist的序号
    int nums;                 // 需要排序的数字个数
    int layer;                // 递归层次
    int position;             // 目前需要寻找的基站的排名
};

void K_dist::print_dists() const // 打印当前的all_dist的状态
{
    cout << "k_dists nums = " << this->nums << endl;
    for (int i = 0; i < this->nums; i++)
    {
        cout << this->all_dists[i] << " ";
    }
    cout << endl;
}

void K_dist::print_names() const // 打印当前的names的状态
{
    for (int i = 0; i < this->nums; i++)
    {
        cout << this->names[i] << " ";
    }
    cout << endl;
}

K_dist::K_dist() // 构造函数，初始化数字个数、存储所有dist
{
    cin >> this->nums;
    double t;
    string str;
    for (int i = 0; i < this->nums; i++)
    {
        cin >> str;
        this->names.push_back(str);
    }
    for (int i = 0; i < this->nums; i++)
    {
        cin >> t;
        this->all_dists.push_back(t);
    }
    this->layer = 0;
}

bool K_dist::ascending(const int &l, const int &r) const // 判断当前区间是不是升序排列
{
    int i = l + 1;
    while (i <= r && all_dists[i] >= all_dists[i - 1])
        i++;
    if (i == r + 1)
        return 1;
    else
        return 0;
}

bool K_dist::descending(const int &l, const int &r) const // 判断当前区间是不是降序排列
{
    int i = l + 1;
    while (i <= r && all_dists[i] <= all_dists[i - 1])
        i++;
    if (i == r + 1)
        return 1;
    else
        return 0;
}

int K_dist::find_it(const int &position) // 处理对不同排名的基站的寻找
{
    this->position = position;
    if (this->position >= this->all_dists.size())
    {
        return -1;
    }
    if (this->ascending(0, this->all_dists.size() - 1)) // 如果已经是递增序
    {
        return position;
    }
    else if (this->descending(0, this->all_dists.size() - 1)) // 如果已经是递减序
        return all_dists.size() - position - 1;

    this->position = position;
    int l = 0, r = 0;
    if (this->selected.size() == 0)
    {
        l = 0, r = this->all_dists.size() - 1;
        return RandomizedSelect(l, r);
    }
    else // 如果之前已经排过序，利用一下已排序列
    {
        int i = 0;
        while (i < selected.size() && selected[i] < position)
        {
            i++;
            // cout << "i = " << i << endl;
        }
        if (i == selected.size())
        {
            l = selected[i - 1] + 1;
            r = this->all_dists.size() - 1;
        }
        else
        {
            if (selected[i] == position)
                return position;
            else
            {
                l = selected[i - 1] + 1;
                r = selected[i] - 1;
            }
        }
        this->layer = 0;
        return RandomizedSelect(l, r); // 线性时间选择
    }
}

int K_dist::RandomizedSelect(const int &l, const int &r) // 线性时间选择
{
    this->layer++;
    cout << "layer = " << this->layer << ", l = " << l << ", r = " << r << endl; // 打印当前递归信息
    if (l == r)
    {
        return l;
    }
    if (this->ascending(l, r)) // 判断是不是已经排好序的序列
        return position;
    else if (this->descending(l, r))
        return r + l - position;
    // 在比较一分为二与一分为三的算法的时候，直接以最左端作为基准，不再随机选择
    // else // 随机生成基准
    // {
    //     random_device rd;  // 获取一个随机数种子
    //     mt19937 gen(rd()); // 使用梅森旋算法初始化生成器
    //     uniform_int_distribution<> int_dist(l, r);
    //     int random_int = int_dist(gen);
    //     cout << "随机生成的基准为" << random_int << endl;
    //     this->exchange_it(random_int, l);
    // }
    int i = l + 1, j = r;
    while (i < j) // 将基准置于它该在的位置
    {
        while (i < j && this->all_dists[i] <= this->all_dists[l])
        {
            i++;
        }
        while (i < j && this->all_dists[j] >= this->all_dists[l])
        {
            j--;
        }
        if (i < j)
        {
            this->exchange_it(i, j);
        }
    }
    while (this->all_dists[i] >= this->all_dists[l] && i > l)
    {
        i--;
    }
    this->exchange_it(i, l); // i是这次QuickSort中选出来的基准数字的位置
    this->selected.push_back(i);
    int insert_pos = 0;
    while (insert_pos < selected.size() && selected[insert_pos] <= selected.back())
    {
        insert_pos++;
    }
    if (insert_pos < selected.size())
    {
        int insert_content = selected.back();
        for (int move_it = selected.size() - 1; move_it > insert_pos; move_it--)
        {
            selected[move_it] = selected[move_it - 1];
        }
        selected[insert_pos] = insert_content;
    }
    // 一分为三
    if (i < this->position)
    {
        while (i > 0 && (all_dists[i - 1] == all_dists[i]))
            i--;
        return RandomizedSelect(i + 1, r);
    }
    else if (i > this->position)
    {
        while (i < all_dists.size() && (all_dists[i + 1] == all_dists[i]))
            i++;
        return RandomizedSelect(l, i - 1);
    }
    return i;
}

void K_dist::exchange_it(const int &a, const int &b) // 交换两个数及基站名
{
    double t;
    string str;
    t = this->all_dists[a];
    this->all_dists[a] = this->all_dists[b];
    this->all_dists[b] = t;
    str = this->names[a];
    this->names[a] = this->names[b];
    this->names[b] = str;
}

int K_dist::check_nums() const
{
    return this->nums;
}

void K_dist::printit(const int &ans) const
{
    cout << "第" << this->position + 1 << "小的基站是" << this->names[ans] << ": " << this->all_dists[ans] << endl;
}

int main()
{
    K_dist k_dists;
    int ans = k_dists.find_it(1 - 1); // 最小的k_dist的基站
    if (ans != -1)
        k_dists.printit(ans);
    ans = k_dists.find_it(5 - 1);
    if (ans != -1)
        k_dists.printit(ans);
    ans = k_dists.find_it(20 - 1);
    if (ans != -1)
        k_dists.printit(ans);
    ans = k_dists.find_it(k_dists.check_nums() - 1); // 最大的
    if (ans != -1)
        k_dists.printit(ans);
    return 0;
}
