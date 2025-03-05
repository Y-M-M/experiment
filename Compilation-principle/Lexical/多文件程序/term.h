#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "String.h"
#include "Charcon.h"
#include "Alpha.h"
#include "Delimiter.h"
#include "Operator.h"
#include "Number.h"
using namespace std;

class Read_word
{
public:
    Read_word(const string &filename);  // 构造函数，打开文件
    bool reach_end() const;             // 判断是否到文档尾
    void solve_it();                    // 对C语言源文件进行处理
    bool Is_signal_operator() const;    // 判断是不是operator
    bool Is_delimter() const;           // 判断是不是delimter
    bool Is_word_end() const;           // 判断当前符号能不能隔开两个单词
    void process_buffer();              // 处理当前buffer中存储的单词
    void read_string();                 // 阅读字符串word
    bool is_modify_string() const;      // 开头是否是修饰字符串的字符
    void read_charcon();                // 阅读字符word
    bool is_alpha(const char &c) const; // 判断输入符号是不是字母
    bool is_num(const char &c) const;   // 判断输入符号是不是数字
    void skipit();                      // 忽略掉注释
    bool read_num();                    // 阅读数字

private:
    vector<char> buffer;
    ifstream file;
    char c;
};
