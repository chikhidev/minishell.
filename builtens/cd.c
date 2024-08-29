#include "main.h"
#include "builtens.h"
#include "parsing.h"
#include "string.h"

int go_home(t_db    *db, char    *args[])
{
	DIR	*to;
	char *pwd;
	t_exp_list *pwd_exp_node;
	t_env_list *pwd_env_node;
	t_env_list *old_pwd_env_node;
	t_exp_list *old_pwd_exp_node;
	t_env_list *home_node;
	pwd_exp_node = get_exp_node(db->exp_list, "PWD");
	pwd_exp_node = get_exp_node(db->exp_list, "PWD");
	pwd_env_node = get_env_node(db->env_list, "PWD");
	old_pwd_exp_node = get_exp_node(db->exp_list, "OLDPWD");
	old_pwd_env_node = get_env_node(db->env_list, "OLDPWD");
	home_node = get_env_node(db->env_list, "HOME");
	if (home_node == NULL)
		return (printf("cd: home not set\n"), 1);
	else
	{
		to = opendir(home_node->val);
		if (to == NULL)
			return (perror(args[1]), 1);
		pwd = getcwd(NULL, 0);
		if (pwd == NULL)
			return (perror("cd"), 1);
		if (old_pwd_env_node)
			old_pwd_env_node->val = ft_strdup_ec(db, pwd);
		if (old_pwd_env_node && old_pwd_env_node->val == NULL)
			return (perror("malloc"), 1);
		if (old_pwd_exp_node)
			old_pwd_exp_node->val = ft_strdup_ec(db, pwd);
		if (old_pwd_exp_node && old_pwd_exp_node->val == NULL)
			return (perror("malloc"), 1);
		if (chdir(home_node->val) == -1)
			return (printf("cd: home not set\n"), 1);
		free(pwd);
		pwd = getcwd(NULL, 0);
		if (pwd_env_node)
		{
			if (!pwd_env_node->val)
				return (printf("cd: home not set\n"), 1);
			pwd_env_node->val = ft_strdup_ec(db, pwd);
		}
		if (pwd_env_node && pwd_env_node->val == NULL)
			return (perror("malloc"), 1);
		if (pwd_exp_node)
		{
			if (!pwd_exp_node->val)
				return (printf("cd: home not set\n"), 1);
			pwd_exp_node->val = ft_strdup_ec(db, pwd);
		}
		if (pwd_exp_node && pwd_exp_node->val == NULL)
			return (perror("malloc"), 1);
		closedir(to);
		free(pwd);
	}
	return (0);
}
int	go_dir(t_db    *db, char    *args[])
{
	DIR	*to;
	char *pwd;
	t_exp_list *pwd_exp_node;
	t_env_list *pwd_env_node;
	t_env_list *old_pwd_env_node;
	t_exp_list *old_pwd_exp_node;

	pwd_exp_node = get_exp_node(db->exp_list, "PWD");
	pwd_exp_node = get_exp_node(db->exp_list, "PWD");
	pwd_env_node = get_env_node(db->env_list, "PWD");
	old_pwd_exp_node = get_exp_node(db->exp_list, "OLDPWD");
	old_pwd_env_node = get_env_node(db->env_list, "OLDPWD");
	char *token;

	token = whithout_quotes_ec(db, args[1]);
	to = opendir(token);
	if (to == NULL)
		return (dprintf(2, "cd: "), perror(args[1]), 1);
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
			return (perror("cd"), 1);
	if (old_pwd_env_node)
			old_pwd_env_node->val = ft_strdup_ec(db, pwd);
	if (old_pwd_env_node && old_pwd_env_node->val == NULL)
		return (perror("malloc"), 1);
	if (old_pwd_exp_node)
		old_pwd_exp_node->val = ft_strdup_ec(db, pwd);
	if (old_pwd_exp_node && old_pwd_exp_node->val == NULL)
		return (perror("malloc"), 1);
	if (chdir(token) == -1)
		return (perror("cd"),closedir(to), 1);
	free(pwd);
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (perror("cd"),closedir(to), 1);
	if (pwd_env_node)
		pwd_env_node->val = ft_strdup_ec(db, pwd);
	if (pwd_env_node && pwd_env_node->val == NULL)
		return (perror("malloc"), closedir(to), 1);
	if (pwd_exp_node)
		pwd_exp_node->val = ft_strdup_ec(db, pwd);
	if (pwd_exp_node && pwd_exp_node->val == NULL)
		return (perror("malloc"),closedir(to), 1);
	closedir(to);
	free(pwd);
	return (0);
}
int cd_(t_db    *db, char    *args[])
{
	
	if (count_array_len(args) > 2)
		return (printf("cd : too many arguments\n"), 1);
	else if (count_array_len(args) == 1)
		return (go_home(db, args));
	else
		return (go_dir(db, args));
	return (0);
}