#include "PDAtoCFG.h"

int main()
{
    PDA pda;
    //cout << pda;
    CFG cfg = pda.generate_cfg();
    cout << cfg;
    return 0;
}