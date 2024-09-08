// #include "./index.h"
#include "main.h"
#include "parsing.h"

char	*get_environment_var(t_db *db, char *var, char *env[])
{
	int		i;
	char	*temp;

	i = 0;
	temp = ft_strjoin(db, var, "=");
	while (env[i])
	{
		if (ft_strncmp(temp, env[i], ft_strlen(temp)) == 0)
		{
			return (env[i] + ft_strlen(var) + 1);
		}
		i++;
	}
	return (NULL);
}

char	*get_env(t_db *db, char *name)
{
	t_exp_list	*object;

	if (!name)
		return (NULL);
	if (ft_strncmp(name, "?", ft_strlen(name)) == 0)
		return (ft_itoa(db, db->last_status));
	if (ft_strncmp(name, "_", ft_strlen(name)) == 0)
		return (ft_strdup(db, get_env_node(db->env_list, "_")->val));
	if (are_all(name, ft_isdigit))
		return (ft_strdup(db, name + 1));
	object = get_exp_node(db->exp_list, name);
	if (object)
	{
		return (ft_strdup(db, object->val));
	}
	return (ft_strdup(db, ""));
}

int	valid_char(char c, int index)
{
	(void)index;
	if (isdigit(c) || ft_isalnum(c))
		return (true);
	if (c == '?' || c == '_')
		return (true);
	return (false);
}

int	concat_env_name(char **line, char **env_var_name, int *i, t_quote *q)
{
	char	*tmp;
	t_db	*db;

	db = this();
	// printf("last status: %d\n", db->last_status);
	tmp = NULL;
	while ((*line)[(*i)] && valid_char((*line)[*i], *i) && !is_quote(q, *i))
	{
		tmp = concat(db, *env_var_name, (*line)[(*i)]);
		*env_var_name = tmp;
		(*i)++;
	}
	// printf("len: %d\n", len);
	return (SUCCESS);
}
