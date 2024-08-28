#include "../includes/main.h"

void    *pid_add(t_db *db, pid_t  new_ip)
{
    t_int    *ip;

    if (!db->pid)
    {
        db->pid = gc_malloc(db, sizeof(t_int));
        if (!db->pid)
            return (NULL);
        db->pid->n = new_ip;
        db->pid->next = NULL;
    }
    else
    {

        ip = db->pid;
        while (ip->next)
            ip = ip->next;
        ip->next = gc_malloc(db, sizeof(t_int));
        if (!ip->next)
            return (NULL);
        ip->next->n = new_ip;
        ip->next->next = NULL;
    }
    return (NULL);
}

void    pid_free(t_db *db, pid_t  ip_to_free)
{
    t_int    *ip;
    t_int    *prev;

    ip = db->pid;
    prev = NULL;
    while (ip)
    {
        if (ip->n == ip_to_free)
        {
            if (prev)
                prev->next = ip->next;
            else
                db->pid = ip->next;
            gc_free(db, ip);
            return ;
        }
        prev = ip;
        ip = ip->next;
    }
}

void    pid_void(t_db *db)
{
    t_int    *ip;

    while (db->pid)
    {
        ip = db->pid;
        db->pid = db->pid->next;
        gc_free(db, ip);
    }
    db->pid = NULL;
}

void    *fd_add(t_db *db, pid_t  new_fd)
{
    t_int    *ip;

    if (!db->fd)
    {
        db->fd = gc_malloc(db, sizeof(t_int));
        if (!db->fd)
            return (NULL);
        db->fd->n = new_fd;
        db->fd->next = NULL;
    }
    else
    {
        ip = db->fd;
        while (ip->next)
            ip = ip->next;
        ip->next = gc_malloc(db, sizeof(t_int));
        if (!ip->next)
            return (NULL);
        ip->next->n = new_fd;
        ip->next->next = NULL;
    }
    return (NULL);
}

void    fd_free(t_db *db, pid_t  fd_to_free)
{
    t_int    *fd;
    t_int    *prev;

    fd = db->fd;
    prev = NULL;
    while (fd)
    {
        if (fd->n == fd_to_free)
        {
            if (prev)
                prev->next = fd->next;
            else
                db->fd = fd->next;
            gc_free(db, fd);
            return ;
        }
        prev = fd;
        fd = fd->next;
    }
}

void    fd_void(t_db *db)
{
    t_int    *fd_head;
    t_int    *tmp;

    fd_head = db->fd;
    while (db->fd)
    {
        tmp = db->fd->next;
        if (db->fd->n > 2)
            close(db->fd->n);
        gc_free(db, db->fd);
        db->fd = tmp;
    }
    db->fd = NULL;
}

