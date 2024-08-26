#include "main.h"
#include "parsing.h"

/*
    good = 1 means in good place (befor and after)
*/

/* flag is -2 if needs something after  op                 '>'    '<<'    '>>'     '<'    '|'          */
/* flag is -1 if needs something befor & after op like    '&&'   '||'                                  */



int is_valid_op(char c, char next_c)
{
    return ((c == '|' && next_c && next_c == '|'));
}
