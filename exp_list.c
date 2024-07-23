#include "main.h"
#include "builtens.h"
#include "parsing.h"
#include "string.h"


t_exp_list *new_exp_node(t_db *db, char   *key, char    *val)
{
    (void)db;
    t_exp_list *new;

    new = malloc(sizeof(t_exp_list));
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
void	push_exp_back(t_exp_list  **list,  t_exp_list	*new)
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