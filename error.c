#include "includes/main.h"
#include "includes/exec.h"

int	error(t_db *db, char *specifier, char *msg)
{
	if (msg)
	{
		if (!specifier)
		{
			ft_write(db, 2, msg, ft_strlen(msg));
			ft_write(db, 2, "\n", 1);
		}
		else
		{
			ft_write(db, 2, specifier, ft_strlen(specifier));
			ft_write(db, 2, ": ", 2);
			ft_write(db, 2, msg, ft_strlen(msg));
			ft_write(db, 2, "\n", 1);
		}
	}
	fd_void(db);
	gc_void(db);
	db->error = true;
	db->last_status = 2;
	return (FAILURE);
}
