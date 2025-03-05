#pragma
#include <iostream>
#include <string>
#include <vector>

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

class Alpha
{
public:
    Alpha(const vector<char> &buffer);
    void judgeit();
    bool is_alpha(const char &c) const;
    bool is_num(const char &c) const;
    friend ostream &operator<<(ostream &Out, const Alpha &output);

private:
    string read_alpha;
    bool is_keyword;
    bool is_identifier;
    int line;
};
