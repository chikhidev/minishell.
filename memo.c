#include "includes/main.h"

void    *gc_malloc(t_db *db, size_t size)
{
    void    *ptr;

    ptr = malloc(size);
    if (!ptr) return (NULL);
    if (!db->gc)
    {
        db->gc = malloc(sizeof(t_gc));
        if (!db->gc) return (NULL);
        db->gc->ptr = ptr;
        db->gc->next = NULL;
    }
    else
    {
        t_gc    *tmp;

        tmp = db->gc;
        while (tmp->next) tmp = tmp->next;
        tmp->next = malloc(sizeof(t_gc));
        if (!tmp->next) return (NULL);
        tmp->next->ptr = ptr;
        tmp->next->next = NULL;
    }
    return (ptr);
}

void    gc_free(t_db *db)
{
    t_gc    *tmp;

    while (db->gc)
    {
        tmp = db->gc;
        db->gc = db->gc->next;
        free(tmp->ptr);
        free(tmp);
    }
}
