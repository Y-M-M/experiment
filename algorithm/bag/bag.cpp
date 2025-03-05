#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef struct item
{
    int id;
    int weight;
    int value;
} Item;

bool compare(Item a, Item b)
{
    return a.weight < b.weight;
}

int main()
{
    int vol;
    cin >> vol;
    int num;
    cin >> num;
    vector<Item> items(num);
    for (int i = 0; i < num; i++)
    {
        items[i].id = i;
        cin >> items[i].weight;
    }
    for (int i = 0; i < num; i++)
    {
        cin >> items[i].value;
    }
    // cout << "the first record is " << items[0].weight << ", " << items[0].value << endl;
    sort(items.begin(), items.end(), compare); // 按重量从小到大排序
    vector<vector<int> > allvalue(num, vector<int>(vol + 1, 0));
    vector<vector<bool> > isget(num, vector<bool>(vol + 1, false)); // 记录最优情况下有没有使用当前物品
    if (items[0].weight <= vol)
    {
        for (int j = items[0].weight; j <= vol; j++)  // 初始化最简子问题
        {
            allvalue[0][j] = items[0].value;
            isget[0][j] = true;
        }
    }
    
    for (int i = 1; i < num; i++)
    {
        int nowweight = items[i].weight;
        int nowvalue = items[i].value;
        for (int j = 0; j <= vol && j < nowweight; j++)
        {
            allvalue[i][j] = allvalue[i - 1][j];
        }
        if (nowweight <= vol)
        {
            for (int j = nowweight; j <= vol; j++)
            {
                if (allvalue[i - 1][j] < allvalue[i - 1][j - nowweight] + nowvalue)
                {
                    isget[i][j] = true;
                    allvalue[i][j] = allvalue[i - 1][j - nowweight] + nowvalue;
                }
                else 
                {
                    allvalue[i][j] = allvalue[i - 1][j];
                }
            }
        }
    }
    cout << "the max value is " << allvalue[num - 1][vol] << endl;
    vector<bool> final_get(num, false);
    int row = num - 1;
    int col = vol;
    int allweight = 0;
    while (row >= 0 && col >= 0)
    {
        if (isget[row][col])
        {
            int itemid = items[row].id;
            final_get[itemid] = true;
            allweight += items[row].weight;
            col = col - items[row].weight;
            row--;
        }
        else
        {
            row--;
        }
    }
    cout << "the weight is " << allweight << endl;
    for (int i = 0; i < num; i++)
    {
        if (final_get[i])
            cout << i << " ";
    }
    cout << endl;
    return 0;
}