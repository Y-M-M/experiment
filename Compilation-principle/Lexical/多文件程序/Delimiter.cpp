#include "Delimiter.h"

Delimter::Delimter(const char &c)
{
    this->c = c;
    line = line_num;
    all_num++;
    DELIMITER_num++;
}

ostream &operator<<(ostream &Out, Delimter delimter_word)
{
    Out << delimter_word.line << " <" << "DELIMTER," << delimter_word.c << ">" << endl;
    return Out;
}
