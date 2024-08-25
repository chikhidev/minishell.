#include "includes/main.h"

int error(t_db *db, char *specifier, char *msg)
{
    if (msg)
    {
        if (!specifier)
           dprintf(2, "%s\n", msg);
        else
            dprintf(2, "%s: %s\n", specifier, msg);
    }
    gc_void(db);
    db->error = true;
    return (FAILURE);
}
