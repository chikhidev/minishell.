#include "main.h"
#include "builtens.h"
#include "parsing.h"
#include "string.h"


t_exp_list *new_exp_node(t_db *db, char   *key, char    *val)
{
    t_exp_list *new;

    new = ec_malloc(db, sizeof(t_exp_list));

    if (new == NULL)
        return (NULL);
    new->val = val;
    new->key = key;
    new->next = NULL;
    return (new);
}

void    add_exp_front(t_exp_list  **list,   t_exp_list	*new)
{
	if (list && new)
	{
		new->next = *list;
		*list = new;
	}
}
void	push_exp_sort(t_exp_list  **list,  t_exp_list	*new)
{
	t_exp_list	*curr;
	t_exp_list	*prev;

	if (list)
	{
		if (*list == NULL)
			add_exp_front(list, new);
        else if (ft_strncmp(new->key, (*list)->key, ft_strlen((*list)->key)) < 0)
			add_exp_front(list, new);
		else
		{
            prev = *list;
			curr = prev->next;
			while (curr && curr->next)
            {
                if (ft_strncmp(new->key, curr->key, ft_strlen(curr->key)) < 0)
                {
                    prev->next = new;
                    new->next = curr;
                    return;
                }
                prev = curr;
				curr = curr->next;
            }
            curr->next = new;
		}
	}
}

void	push_exp_back(t_exp_list  **list,  t_exp_list	*new)
{
	t_exp_list	*curr;

	if (list)
	{
		if (*list == NULL)
        {
            add_exp_front(list, new);
        }
        else if (ft_strncmp(new->key, (*list)->key, ft_strlen((*list)->key)) < 0)
			add_exp_front(list, new);
		else
		{
			curr = (*list);
			while (curr && curr->next)
				curr = curr->next;
            curr->next = new;
		}
	}
}

t_exp_list  *get_exp_node(t_exp_list    *list,  char    *key)
{
    t_exp_list  *curr;

    curr = list;
    while (curr)
    {
        if (ft_strcmp(curr->key, key) == 0)
            return curr;
        curr = curr->next;
    }
    return curr;
}

void  del_exp_node(t_exp_list    **list,  char    *key)
{
    t_exp_list  *curr;
    t_exp_list  *prev;

    prev = *list;
	if (prev)
	{
		if (ft_strcmp(prev->key, key) == 0)
		{
			*list = (*list)->next;
			return ;
		}
    	curr = prev->next;
    	while (curr)
		{
			if (ft_strcmp(curr->key, key) == 0)
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