#include "main.h"
#include "builtens.h"
#include "parsing.h"
#include "string.h"

void set_pwd_str_values(t_db *db, char	**pwd_str, char **old_pwd_str)
{
	t_env_list *pwd_env;
	t_env_list *old_pwd_env;

	pwd_env = get_env_node(db->env_list, "PWD");
	old_pwd_env = get_env_node(db->env_list, "OLDPWD");
	if (pwd_env)
		*pwd_str = ft_strdup_ec(db, pwd_env->val);
	if (old_pwd_env)
		*old_pwd_str = ft_strdup_ec(db, old_pwd_env->val);

}

void set_pwd_env_values(t_db *db, char	*old_pwd_str)
{
	t_env_list *pwd_env;
	t_env_list *old_pwd_env;

	t_exp_list *pwd_exp;
	t_exp_list *old_pwd_exp;

	char *new_pwd;

	new_pwd = getcwd(NULL, 0);
	pwd_env = get_env_node(db->env_list, ft_strdup_ec(db, "PWD"));
	old_pwd_env = get_env_node(db->env_list, ft_strdup_ec(db, "OLDPWD"));
	pwd_exp = get_exp_node(db->exp_list, ft_strdup_ec(db, "PWD"));
	old_pwd_exp = get_exp_node(db->exp_list, ft_strdup_ec(db, "OLDPWD"));
	if (pwd_env)
		pwd_env->val = ft_strdup_ec(db, new_pwd);
	else
		push_env_back(&db->env_list, new_env_node(db, ft_strdup_ec(db, "PWD"), getcwd(NULL, 0)));
	if (old_pwd_env)
		old_pwd_env->val = ft_strdup_ec(db, old_pwd_str);
	else
		push_env_back(&db->env_list, new_env_node(db, ft_strdup_ec(db, "OLDPWD"), ft_strdup_ec(db, old_pwd_str)));
	if (pwd_exp)
		pwd_exp->val = ft_strdup_ec(db, new_pwd);
	else
		push_exp_back(&db->exp_list, new_exp_node(db, ft_strdup_ec(db, "PWD"), getcwd(NULL, 0)));
	if (old_pwd_exp)
		old_pwd_exp->val = ft_strdup_ec(db, old_pwd_str);
	else
		push_exp_back(&db->exp_list, new_exp_node(db, ft_strdup_ec(db, "OLDPWD"), ft_strdup_ec(db, old_pwd_str)));
	free(new_pwd);
}

int change_dir(t_db *db, char *args[])
{
	char *dest;
	char	*pwd_str;
	char	*oldpwd_str;

	pwd_str = NULL;
	oldpwd_str = NULL;
	set_pwd_str_values(db, &pwd_str, &oldpwd_str);
	t_exp_list *home_exp;

	home_exp = get_exp_node(db->exp_list, "HOME");
	if (count_array_len(args) == 1)
	{
		if (!home_exp)
			return (dprintf(2, "cd : Home is not set"), 1);
		dest = home_exp->val;
	}
	else
		dest = args[1];
	if (chdir(dest) == -1)
		return (dprintf(2, "cd: "), perror(dest), 1);
	set_pwd_env_values(db, pwd_str);
	
	return 0;
}

int cd_(t_db    *db, char    *args[])
{
	
	if (count_array_len(args) > 2)
		return (printf("cd : too many arguments\n"), 1);
	return (change_dir(db, args));
}