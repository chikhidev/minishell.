#include "builtens.h"
#include "main.h"
#include <errno.h>

int	pwd_(t_db *db)
{
	(void)db;
	char *pwd;
	t_exp_list *pwd_exp_node;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		pwd_exp_node = get_exp_node(db->exp_list, "PWD");
		if (!pwd_exp_node)
			return (1);
		printf("%s\n", pwd_exp_node->val);
	}
	else
		printf("%s\n", pwd);
	free(pwd);
	return (0);
}