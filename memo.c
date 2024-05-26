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

void    gc_free(t_db *db, void *ptr)
{
    t_gc    *tmp;
    t_gc    *prev;

    tmp = db->gc;
    prev = NULL;
    while (tmp)
    {
        if (tmp->ptr == ptr)
        {
            if (prev)
                prev->next = tmp->next;
            else
                db->gc = tmp->next;
            free(tmp->ptr);
            free(tmp);
            return ;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

void    gc_void(t_db *db)
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

void    *gc_realloc(t_db *db, void *ptr, size_t size)
{
    t_gc    *tmp;

    tmp = db->gc;
    while (tmp)
    {
        if (tmp->ptr == ptr)
        {
            void    *new_ptr;

            new_ptr = realloc(ptr, size);
            if (!new_ptr) return (NULL);
            tmp->ptr = new_ptr;
            return (new_ptr);
        }
        tmp = tmp->next;
    }
    return (NULL);
}
