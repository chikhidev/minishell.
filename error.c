#include "includes/main.h"

int error(t_db *db, char *specifier, char *msg)
{
    if (msg)
    {
        if (!specifier)
           printf("%s\n", msg);
        else
            printf("%s: %s\n", specifier, msg);
    }
    gc_void(db);
    db->error = true;
    return (FAILURE);
}
