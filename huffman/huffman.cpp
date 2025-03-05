#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

typedef struct node  // 哈夫曼树的节点
{
    char c;
    int nums;
    struct node* left;
    struct node* right;
} Node;

void printtree(const string& code, Node* root, vector<pair<int, int>>& codesize)
{
    if (root->c != -1 && root->left == nullptr && root->right == nullptr)
    {
        codesize.push_back(make_pair(root->nums, code.length()));
        cout << (int)root->c << " '" << root->c << "': " << "共出现" << root->nums << "次，编码为" << code << endl;
    }
    else 
    {
        if (root->left != nullptr)
            printtree(code + '0', root->left, codesize);
        if (root->right != nullptr)
            printtree(code + '1', root->right, codesize);
    }
}

void clearit(const Node* root)
{
    if (root == nullptr) return;
    while (root->left != nullptr || root->right != nullptr)
    {
        if (root->left != nullptr)
            clearit(root->left);
        if (root->right != nullptr)
            clearit(root->right);
    }
    delete root;
}

int main()
{
    vector<int> frequency(128, 0);
    vector<pair<int, int>> codesize;
    char c;
    int allnum = 0;
    while ((c = getchar()) != EOF) // 将输入的所有字符读入，并统计每个字符出现的次数
    {
        frequency[c]++;
        allnum++;
    }
    vector<Node*> nodes;
    for (int i = 0; i < 128; i++) // 构建节点
    {
        if (frequency[i])
        {
            Node *anode = new Node;
            anode->c = i;
            anode->nums = frequency[i];
            anode->left = nullptr;
            anode->right = nullptr;
            nodes.push_back(anode);
        }
    }
    int allsignum = nodes.size();
    cout << "共出现" << allsignum << "个字符。" << endl;
    int codelength = 0;
    int range = 1;
    while ((range *= 2) < allsignum)
    {
        codelength++;
    }

    auto cmp = [](const Node* node1, const Node* node2)
    {
        return node1->nums > node2->nums;
    };
    make_heap(nodes.begin(), nodes.end(), cmp); // 堆排序
    
    while(nodes.size() > 1) // 取出两个出现次数最小的节点，组成新的节点，直到只剩最后一个节点——根节点为止
    {
        Node* newnode = new Node;
        newnode->nums = 0;
        newnode->c = -1;

        pop_heap(nodes.begin(), nodes.end(), cmp);
        newnode->nums += nodes.back()->nums;
        newnode->left = nodes.back();
        nodes.pop_back();

        pop_heap(nodes.begin(), nodes.end(), cmp);
        newnode->nums += nodes.back()->nums;
        newnode->right = nodes.back();
        nodes.pop_back();

        nodes.push_back(newnode);
        push_heap(nodes.begin(), nodes.end(), cmp);
    }
    string code;
    printtree(code, nodes[0], codesize); // 统计每个字符的编码长度并输出其哈夫曼编码、出现次数
    int huffmanlen = 0;
    for (int i = 0; i < codesize.size(); i++) // 统计采用哈夫曼编码需要的比特数
    {
        huffmanlen += codesize[i].first * codesize[i].second;
    }
    cout << "采用哈夫曼编码需要的存储比特数为: " << huffmanlen << endl;
    cout << "采用定长编码需要的存储比特数为: " << codelength * allnum << endl;
    cout << "如果将input.txt中未出现的符号(ASCII码中)也考虑在内的话，需要的存储比特数为：" << 7 * allnum << endl;
    return 0;
}