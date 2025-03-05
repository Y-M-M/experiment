// 以城市20为出发点，求解TSP问题
// 回溯法
// 对比两个算法的空间复杂性、时间复杂性
// 记录 从起始城市出发的最短旅行路径， 路径总长度， 扫描过的搜索树结点总数L， 程序运行时间T

#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
using namespace std;

class Cities {
public:
    Cities(int n);

private:
    vector<int> id;
    vector<vector<int>> road;
};

int main()
{
    
}

