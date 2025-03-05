#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <priority_queue>
using namespace std;

int main()
{
    vector<int> frequency(128, 0);
    char c;
    while (c = getchar() != EOF)
    {
        frequency[c]++;
    }
    
}