#include "../includes/exec.h"
#include "../includes/parsing.h"
#include "../includes/main.h"
#include "../includes/string.h"

t_file_entry *new_ent_node(t_db *db, struct dirent  *ent)
{
    t_file_entry *new;
    new = gc_malloc(db,  sizeof(t_file_entry));
    if (new == NULL)
        return (NULL);
    new->next = NULL;
    new->entry = ent;
    return (new);
}
void    add_ent_front(t_file_entry  **list,   t_file_entry	*new)
{
	if (list && new)
	{
		new->next = *list;
		*list = new;
	}
}
void	push_ent_back(t_file_entry  **list,  t_file_entry	*new)
{
	t_file_entry	*last;

	if (list)
	{
		if (*list == NULL)
			add_ent_front(list, new);
		else
		{
			last = *list;
			while (last && last->next)
				last = last->next;
			last->next = new;
		}
	}
}
t_file_entry  *get_ent_node(t_file_entry    *list,  char    *name,  unsigned char   type)
{
    t_file_entry  *curr;

    curr = list;
    while (curr)
    {
        if (ft_strcmp(curr->entry->d_name, name) == 0 && curr->entry->d_type == type)
            return curr;
        curr = curr->next;
    }
    return curr;
}

void  del_ent_node(t_file_entry    **list,  char    *name, unsigned char    type)
{
    t_file_entry  *curr;
    t_file_entry  *prev;

    prev = *list;
	if (prev)
	{
		if (ft_strcmp(name, prev->entry->d_name) == 0 && prev->entry->d_type == type)
		{
			*list = (*list)->next;
			return ;
		}
    	curr = prev->next;
    	while (curr)
		{
		    if (ft_strcmp(name, curr->entry->d_name) == 0 && curr->entry->d_type == type)
			{
				prev->next = curr->next;
				return ;
			}
			prev = curr;
			curr = curr->next;
		}
	}
    return;
}