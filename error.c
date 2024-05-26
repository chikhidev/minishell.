#include "includes/main.h"

int error(t_db *db, char *msg)
{
    printf(RED"Error: %s\n"RESET, msg);

    (void)db;
    gc_free(db);
    return (FAILURE);
}
