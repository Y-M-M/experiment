#pragma
#include <iostream>
#include <vector>
#include <string>
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

class Delimter
{
public:
    Delimter(const char &c);
    friend ostream &operator<<(ostream &Out, Delimter delimter_word);

private:
    int line;
    char c;
};
