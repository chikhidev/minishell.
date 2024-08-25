#include "main.h"
#include "string.h"
#include "parsing.h"
#include "builtens.h"

t_env_list *new_env_node(t_db *db, char   *key, char    *val)
{
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
			while (last && last->next)
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

void  del_env_node(t_env_list    **list,  char    *key)
{
    t_env_list  *curr;
    t_env_list  *prev;

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

char	**env_list_to_env_arr(t_db	*db)
{
	t_env_list	*env_list;
	char	**env_arr;
	int	env_len;
	env_len = 0;
	char	*temp;

	env_arr = NULL;
	env_list = db->env_list;
	while (env_list)
	{
		env_list = env_list->next;
		env_len++;
	}
	env_arr = ec_malloc(db, sizeof(char	*) * (env_len + 1));
	if (!env_arr)
		return (NULL);
	env_list = db->env_list;
	env_len = 0;
	while (env_list)
	{
		env_arr[env_len] = ec_malloc(db, ft_strlen(env_list->key) + 1 + ft_strlen(env_list->val) + 1);
		if (!env_arr[env_len])
			return (NULL);
		temp = ft_strjoin(db, env_list->key, "=");
		if (!temp)
			return (NULL);
		temp = ft_strjoin(db, temp, env_list->val);
		if (!temp)
			return (NULL);
		env_arr[env_len++] = temp;
		env_list = env_list->next;
	}

	env_arr[env_len] = NULL;
	return env_arr;
}