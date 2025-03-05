// 用最大和减去最小和
// 存在的问题：如果最小和出现在最大和的右边，则会出错
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> nums;
    int num;
    while (scanf("%d", &num) != EOF)
    {
        nums.push_back(num);
    }
    int n = nums.size();
    int maxsum = 0;
    int minsum = 0;
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += nums[i];
        maxsum = max(maxsum, sum);
        minsum = min(minsum, sum);
    }
    if (minsum < 0)
        cout << maxsum - minsum << endl;
    else
        cout << maxsum;
    return 0;
}