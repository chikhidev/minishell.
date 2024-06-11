#include "includes/main.h"

int error(t_db *db, char *msg)
{
    printf("Error: %s\n\n", msg);
    gc_void(db);
    return (FAILURE);
}
