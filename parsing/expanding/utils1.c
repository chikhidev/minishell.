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

// int	valid_char(char c, int index)
// {
// 	// (void)index;
// 	if ((isdigit(c) && index > 1))
// 		return (false);

// 	if (ft_isalnum(c))
// 		return (true);
// 	if (c == '?' || c == '_')
// 		return (true);
// 	return (false);
// }

int	concat_env_name(char **line, char **env_var_name, int *i, t_quote *q)
{
	char	*tmp;
	t_db	*db;
	bool 	is_digit;
	bool 	is_alpha;
	bool 	is_special_char;


	db = this();
	tmp = NULL;

	is_alpha = false;
	is_digit = false;
	is_special_char = false;

	while ((*line)[(*i)] && !is_quote(q, *i))
	{
		if (!ft_isalnum((*line)[(*i)]) && (*line)[(*i)] != '?' && (*line)[(*i)] != '_')
			return (FAILURE);
		if (is_special_char)
			return FAILURE;
		if (ft_isdigit((*line)[(*i)]))
			is_digit = true;
		if ((*line)[(*i)] == '?' || (*line)[(*i)] == '_')
			is_special_char = true;
		if (ft_isalpha((*line)[(*i)]))
		{
			if (is_digit)
				return FAILURE;
			is_alpha = true;
		}

		tmp = concat(db, *env_var_name, (*line)[(*i)]);
		*env_var_name = tmp;
		(*i)++;
	}
	// printf("len: %d\n", len);
	return (SUCCESS);
}
