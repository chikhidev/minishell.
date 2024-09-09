#include "builtens.h"
#include "exec.h"
#include "main.h"
#include "string.h"

static int	is_space(char c)
{
	return ((c <= 13 && c >= 9) || c == 32);
}

void skip_atoll(char *str, int *i, int *sign)
{
	while (is_space(str[(*i)]))
		(*i)++;
	if (str[(*i)] == '-' || str[(*i)] == '+')
	{
		if (str[(*i)] == '-')
			(*sign) = -1;
		(*i)++;
	}
}

long long	ft_atoll(char *str, bool *error)
{
	long long	res;
	int			sign;
	int			i;

	sign = 1;
	i = 0;
	res = 0;
	*error = false;
	skip_atoll(str, &i, &sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res * sign > (LLONG_MAX - (str[i] - '0')) / 10 || res
			* sign < (LLONG_MIN + (str[i] - '0')) / 10)
		{
			*error = true;
			return (0);
		}
		res = res * 10 + (str[i] - '0');
		i++;
	}
	skip_spaces((char *)str, &i);
	if (str[i] != '\0')
		*error = true;
	return (res * sign);
}

void setup_exit(t_db *db, int *args_len, char *av[], int index)
{
	(*args_len) = count_array_len(av);

	if ((*args_len) == 1)
	{
		if (index == -1)
			printf("exit\n");
		ft_exit(db, 0, 3, NULL);
	}
}

int exit_check(char *av[], int args_len, int err, unsigned char *exit_code)
{
	if (err)
	{
		dprintf(2, "exit: %s: numeric argument required\n", av[1]);
		*exit_code = 2;
	}
	else if (args_len > 2)
	{
		dprintf(2, "exit: too many arguments\n");
		return (1);
	}
	return (0);
}

int	exit_(t_db *db, char *av[], int index)
{
	unsigned char exit_code;
	int args_len;
	bool err;

	err = false;
	exit_code = 0;
	args_len = 1;
	setup_exit(db, &args_len, av, index);
	exit_code = (unsigned char)ft_atoll(av[1], &err);
	if (exit_check(av, args_len, err, &exit_code) == 1)
		return (1);
	if (db->stdin_dup != -1)
		close(db->stdin_dup);
	if (db->stdout_dup != -1)
		close(db->stdout_dup);
	ft_exit(db, exit_code, 3, NULL);
	return (1);
}
