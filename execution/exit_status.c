#include "../includes/main.h"

void    *ip_add(t_db *db, pid_t  new_ip)
{
    t_ip_addrs    *ip;

    if (!db->ip)
    {
        db->ip = malloc(sizeof(t_ip_addrs));
        if (!db->ip)
            return (NULL);
        db->ip->ip_addr = new_ip;
        db->ip->next = NULL;
    }
    else
    {

        ip = db->ip;
        while (ip->next)
            ip = ip->next;
        ip->next = malloc(sizeof(t_ip_addrs));
        if (!ip->next)
            return (NULL);
        ip->next->ip_addr = new_ip;
        ip->next->next = NULL;
    }
    return (NULL);
}

void    ip_free(t_db *db, pid_t  ip_to_free)
{
    t_ip_addrs    *ip;
    t_ip_addrs    *prev;

    ip = db->ip;
    prev = NULL;
    while (ip)
    {
        if (ip->ip_addr == ip_to_free)
        {
            if (prev)
                prev->next = ip->next;
            else
                db->ip = ip->next;
            free(ip);
            return ;
        }
        prev = ip;
        ip = ip->next;
    }
}


void    ip_void(t_db *db)
{
    t_ip_addrs    *ip;

    while (db->ip)
    {
        ip = db->ip;
        db->ip = db->ip->next;
        free(ip);
    }
    db->ip = NULL;
}
