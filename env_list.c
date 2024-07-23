#include "main.h"

t_env_list *new_env_node(t_db *db, char   *data)
{
	(void)db;
    t_env_list *new;
    new = malloc(sizeof(t_env_list));
    if (new == NULL)
        return (NULL);
    new->data = data;
    new->next = NULL;
	if (data == NULL)
		new->has_val = false;
	else
		new->has_val = true;
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

// void	*push_exp_sort(t_db *db, t_exp_list  **list, char    *data)
// {
// 	t_exp_list	*last;
// 	t_exp_list	*new;

// 	if (list)
// 	{
// 		if (*list == NULL)
// 			new = (t_exp_list *) add_env_front(db, list, data);
// 		else
// 		{
//             new = (t_exp_list *) new_env_node(db, data);
// 			last = *list;
// 			while (last->next)
// 				last = last->next;
// 			last->next = new;
// 		}
// 	}
//     return (new);
// }