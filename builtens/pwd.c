#include "builtens.h"
#include "main.h"
#include <errno.h>

int	pwd_(t_db *db)
{
	(void)db;
	char *pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	if (pwd == NULL)
		return (1);
	free(pwd);
	return (0);
}