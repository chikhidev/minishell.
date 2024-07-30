#include "main.h"
#include "builtens.h"
#include <errno.h>

void pwd(t_db   *db)
{
	(void)db;
	char *pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
}