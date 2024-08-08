#include "main.h"
#include "builtens.h"
#include "parsing.h"
#include "string.h"

int cd_(t_db    *db, char    *args[])
{
	DIR	*to;
	char *pwd;
	char *token;
	t_exp_list *pwd_exp_node;
	t_env_list *pwd_env_node;
	t_env_list *home_node;
	pwd_exp_node = get_exp_node(db->exp_list, "PWD");
	pwd_env_node = get_env_node(db->env_list, "PWD");
	if (count_array_len(args) > 2)
		return (printf("cd : too many arguments"), FAILURE);
	else if (count_array_len(args) == 1)
	{
		home_node = get_env_node(db->env_list, "HOME");
		if (home_node == NULL)
			printf("cd: home is not set\n");
		else
		{
			to = opendir(home_node->val);
			if (to == NULL)
				return (perror(args[1]), FAILURE);
			if (chdir(home_node->val) == -1)
				return (printf("cd: home is not set\n"), FAILURE);
			pwd = getcwd(NULL, 0);
			if (pwd_env_node)
				pwd_env_node->val = ft_strdup_ec(db, pwd);
			if (pwd_env_node && pwd_env_node->val == NULL)
				return (perror("malloc"), FAILURE);
			if (pwd_exp_node)
				pwd_exp_node->val = ft_strdup_ec(db, pwd);
			if (pwd_exp_node && pwd_exp_node->val == NULL)
				return (perror("malloc"), FAILURE);
			
			closedir(to);
			free(pwd);
		}
	}
	else
	{
		token = whithout_quotes_ec(db, args[1]);
		if (!token)
			return (perror("malloc"), FAILURE);
		to = opendir(token);
		if (to == NULL)
			return (perror(args[1]), FAILURE);
		if (chdir(token) == -1)
			return (perror("cd"),closedir(to), FAILURE);
		pwd = getcwd(NULL, 0);
		if (pwd == NULL)
			return (perror("cd"),closedir(to), FAILURE);
		if (pwd_env_node)
			pwd_env_node->val = ft_strdup_ec(db, pwd);
		if (pwd_env_node && pwd_env_node->val == NULL)
			return (perror("malloc"), closedir(to), FAILURE);
		if (pwd_exp_node)
			pwd_exp_node->val = ft_strdup_ec(db, pwd);
		if (pwd_exp_node && pwd_exp_node->val == NULL)
			return (perror("malloc"),closedir(to), FAILURE);
		closedir(to);
		free(pwd);
	}
	return (SUCCESS);
}