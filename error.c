#include "includes/main.h"

int error(t_db *db, char *msg)
{
    printf(RED"Error: %s\n\n"RESET, msg);
    gc_void(db);
    return (FAILURE);
}
