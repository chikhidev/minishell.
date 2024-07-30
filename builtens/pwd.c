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
		(ec_void(db), exit(2)); // fix
	cwd = get_pwd(db);
	if (cwd)
		printf("%s\n", cwd);
}

char	*get_pwd(t_db	*db)
{
	int		len;
	char    *buff;
	char    *cwd;

	len = 10;
	buff = malloc(len);
	if (!buff)
		return (ec_void(db), NULL);
	cwd = getcwd(buff, len); // mind the errno = ERRANGE  (perm denied)
	while (cwd == NULL && len < 4096)
	{
		len = len * 2;
		free(buff);
		buff = malloc(len);
		if (!buff)
			return (ec_void(db), NULL);
		cwd = getcwd(buff, len);
	}
	return (cwd);	
}