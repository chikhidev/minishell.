#include "main.h"
#include "string.h"
#include "parsing.h"

t_env_list *new_env_node(t_db *db, char   *data)
{
	(void)db;
    t_env_list *new;
    char    **key_val;
    new = malloc(sizeof(t_env_list));
    if (new == NULL)
        return (NULL);
    new->next = NULL;
    key_val = ft_split(data, '=');
    // check nyll
    if (!key_val)
        return NULL;
    new->key = key_val[0];
    if (key_val[1])
        new->val = key_val[1];
    else
    {
        new->val = ft_strdup("");
        free(key_val[1]);
    }
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