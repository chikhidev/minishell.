#include "includes/main.h"

void    *gc_malloc(t_db *db, size_t size)
{
    void    *ptr;

    ptr = malloc(size + 1);
    if (!ptr) return (NULL);
    ft_bzero(ptr, size + 1);
    if (!db->gc)
    {
        db->gc = malloc(sizeof(t_gc));
        if (!db->gc)
            return (NULL);
        db->gc->ptr = ptr;
        db->gc->next = NULL;
    }
    else
    {
        t_gc    *gc;

        gc = db->gc;
        while (gc->next)
            gc = gc->next;
        gc->next = malloc(sizeof(t_gc));
        if (!gc->next)
            return (NULL);
        gc->next->ptr = ptr;
        gc->next->next = NULL;
    }
    return (ptr);
}

void    gc_free(t_db *db, void *ptr)
{
    t_gc    *gc;
    t_gc    *prev;

    gc = db->gc;
    prev = NULL;
    while (gc)
    {
        if (gc->ptr == ptr)
        {
            if (prev)
                prev->next = gc->next;
            else
                db->gc = gc->next;
            free(gc->ptr);
            free(gc);
            return ;
        }
        prev = gc;
        gc = gc->next;
    }
}

void    *ec_malloc(t_db *db, size_t size)
{
    void    *ptr;

    ptr = malloc(size);
    if (!ptr)
        return (NULL);
    if (!db->ec)
    {
        db->ec = malloc(sizeof(t_gc));
        if (!db->ec)
            return (NULL);
        db->ec->ptr = ptr;
        db->ec->next = NULL;
    }
    else
    {
        t_gc    *ec;

        ec = db->ec;
        while (ec->next)
            ec = ec->next;
        ec->next = malloc(sizeof(t_gc));
        if (!ec->next)
            return (NULL);
        ec->next->ptr = ptr;
        ec->next->next = NULL;
    }
    return (ptr);
}

void    ec_free(t_db *db, void *ptr)
{
    t_gc    *ec;
    t_gc    *prev;

    ec = db->ec;
    prev = NULL;
    while (ec)
    {
        if (ec->ptr == ptr)
        {
            if (prev)
                prev->next = ec->next;
            else
                db->ec = ec->next;
            free(ec->ptr);
            free(ec);
            return ;
        }
        prev = ec;
        ec = ec->next;
    }
}

void    gc_void(t_db *db)
{
    t_gc    *gc;

    while (db->gc)
    {
        gc = db->gc;
        db->gc = db->gc->next;
        free(gc->ptr);
        free(gc);
    }
    db->gc = NULL;
}

void    ec_void(t_db *db)
{
    t_gc    *ec;

    while (db->ec)
    {
        ec = db->ec;
        db->ec = db->ec->next;
        free(ec->ptr);
        free(ec);
    }
    db->ec = NULL;
}

// void *ft_realloc(t_db    *db,    void    *old_ptr,   size_t  size,  bool free_)
// {
//     void    *new_ptr;

//     new_ptr =   gc_malloc(db,   size + 1);
//     ft_bzero(new_ptr, size);
//     ft_memcpy(new_ptr,  old_ptr, size);
//     if (free_)
//         free(old_ptr);
//     else
//         gc_free(db, old_ptr);
//     return new_ptr;
// }

void    *gc_realloc(t_db *db, void *ptr, size_t size)
{
    t_gc    *gc;

    gc = db->gc;
    while (gc)
    {
        if (gc->ptr == ptr)
        {
            void    *new_ptr;

            new_ptr = realloc(ptr, size);
            if (!new_ptr) return (NULL);
            gc->ptr = new_ptr;
            return (new_ptr);
        }
        gc = gc->next;
    }
    return (NULL);
}
