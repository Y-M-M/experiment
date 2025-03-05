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
    vector<int> sum(n);
    vector<int> pos(n);
    sum[0] = nums[0];
    pos[0] = 0;
    int maxsum = nums[0];
    int lpos = 0;
    int rpos = 0;
    for (int i = 1; i < n; i++)
    {
        if (sum[i - 1] > 0)
        {
            pos[i] = pos[i - 1];
            sum[i] = sum[i - 1] + nums[i];
        }
        else
        {
            pos[i] = i;
            sum[i] = nums[i];
        }
        if (sum[i] > maxsum)
        {
            maxsum = sum[i];
            lpos = pos[i];
            rpos = i;
        }
    }
    cout << "the max sum is: " << maxsum << endl;
    cout << "begin with element " << lpos << ", end with element " << rpos << endl;
    return 0;
}