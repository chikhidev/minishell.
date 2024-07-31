#include "main.h"
#include "builtens.h"
#include <errno.h>

int pwd(t_db   *db)
{
	(void)db;
	char *pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	if (pwd == NULL)
		return FAILURE;
	free(pwd);
	return SUCCESS;
}