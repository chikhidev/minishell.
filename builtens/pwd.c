#include "main.h"
#include "builtens.h"
#include <errno.h>

void pwd(t_db   *db)
{
	int		len;
	char    *buff;
	char    *cwd;

	len = 10;
	buff = malloc(len);
	if (!buff)
		(gc_void(db), exit(2));
	cwd = getcwd(buff, len);
	if (cwd == NULL && errno == ERANGE)
	{
		while (cwd == NULL)
		{
			len = len * 2;
			free(buff);
			buff = malloc(len);
			if (!buff)
				(gc_void(db), exit(2));
			cwd = getcwd(buff, len);
		}
		printf("%s\n", cwd);
		free(cwd);
	}
	else if (cwd == NULL)
		(gc_void(db), perror("pwd"), exit(1));
	else
	{
		printf("%s\n", cwd);
		free(cwd);
	}
}