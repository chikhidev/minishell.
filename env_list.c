#include "main.h"
#include "string.h"
#include "parsing.h"
#include "builtens.h"

t_env_list *new_env_node(t_db *db, char   *key, char    *val)
{
	(void)db;
    t_env_list *new;
    new = ec_malloc(db,  sizeof(t_env_list));
    if (new == NULL)
        return (NULL);
    new->next = NULL;
    new->key = key;
    new->val = val;
    return (new);
}
void    add_env_front(t_env_list  **list,   t_env_list	*new)
{
	if (list && new)
	{
		new->next = *list;
		*list = new;
	}
}
void	push_env_back(t_env_list  **list,  t_env_list	*new)
{
	t_env_list	*last;

	if (list)
	{
		if (*list == NULL)
			add_env_front(list, new);
		else
		{
			last = *list;
			while (last->next)
				last = last->next;
			last->next = new;
		}
	}
}
t_env_list  *get_env_node(t_env_list    *list,  char    *key)
{
    t_env_list  *curr;

    curr = list;
    while (curr)
    {
        if (ft_strcmp(curr->key, key) == 0)
            return curr;
        curr = curr->next;
    }
    return curr;
}