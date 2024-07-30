#include "main.h"
#include "builtens.h"
#include "parsing.h"
#include "string.h"



void cd(t_db    *db, char    *args[])
{
	DIR	*to;
	char *pwd;
	char *token;
	char *home;
	int	flag;
	t_exp_list *pwd_exp_node;
	t_env_list *pwd_env_node;
	pwd_exp_node = get_exp_node(db->exp_list, "PWD");
	pwd_env_node = get_env_node(db->env_list, "PWD");
	if (count_array_len(args) > 2)
		printf("cd : too many arguments");
	else if (count_array_len(args) == 1)
	{
		home = get_environment_var(db, "HOME", db->env);
		if (home == NULL)
			printf("cd: home is not set");
		else
		{
			flag = chdir(home);
			pwd = getcwd(NULL, 0);
			if (flag == -1)
				perror("cd");
			if (pwd_env_node)
				pwd_env_node->val = ft_strdup_ec(db, pwd);
			if (pwd_exp_node)
				pwd_exp_node->val = ft_strdup_ec(db, pwd);
			free(pwd);
		}
	}
	else
	{
		token = whithout_quotes_ec(db, args[1]);
		if (!token)
			return;
		to = opendir(token);
		if (to == NULL)
			return perror(args[1]);
		flag = chdir(token);
		if (flag == -1)
			perror("cd");
		pwd = getcwd(NULL, 0);
		if (pwd_env_node)
			pwd_env_node->val = ft_strdup_ec(db, pwd);
		if (pwd_exp_node)
			pwd_exp_node->val = ft_strdup_ec(db, pwd);
		free(pwd);
	}
}