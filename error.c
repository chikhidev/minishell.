#include "includes/main.h"

int error(t_db *db, char *specifier, char *msg)
{
    if (msg)
    {
        if (!specifier)
           printf(RED"-->:"RESET" %s\n", msg);
        else
            printf(RED"-->: %s:"RESET" %s\n", specifier, msg);
    }
    gc_void(db);
    db->error = TRUE;
    return (FAILURE);
}
