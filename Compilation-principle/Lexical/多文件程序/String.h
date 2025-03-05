#pragma once
#include <vector>
#include <string>
#include <iostream>
using namespace std;

extern int line_num;
extern int KEYWORD_num;
extern int IDENTIFIER_num;
extern int OPERATOR_num;
extern int DELIMITER_num;
extern int CHARCON_num;
extern int STRING_num;
extern int NUMBER_num;
extern int all_num;
extern int ERROR_num;

class String
{
public:
    String(vector<char> &buffer);
    void judgeit();
    friend ostream &operator<<(ostream &Out, String &a_string);

private:
    string read_string;
    bool is_string;
    int line;
};
