#include "main.h"
#include "builtens.h"
#include "string.h"

static int	is_space(char c)
{
	return ((c <= 13 && c >= 9) || c == 32);
}

#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

long long ft_atoll(const char *str, bool *error)
{
    long long res;
    int sign;
    int i;

    sign = 1;
    i = 0;
    res = 0;
    *error = false;
    // Skip whitespace
    while (is_space(str[i]))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        if (res * sign > (LLONG_MAX - (str[i] - '0')) / 10 || res * sign < (LLONG_MIN + (str[i] - '0')) / 10)
        {
            *error = true;
            return 0;
        }
        res = res * 10 + (str[i] - '0');
        i++;
    }
    skip_spaces((char *)str ,&i);
    if (str[i] != '\0')
        *error = true;
    return res * sign;
}

int exit_(t_db  *db,    char    *av[])
{
    unsigned    char    exit_code;
    int args_len;
    bool err;

    err = false;
    exit_code = 0;
    args_len = count_array_len(av);

    if (args_len == 1)
        exit(0);
    exit_code = (unsigned char)ft_atoll(av[1], &err);
    if (err)
    {
        dprintf(2, "exit: %s: numeric argument required\n", av[1]);
        exit_code = 2;
    }
    else if (args_len > 2)
    {
        dprintf(2, "exit: too many arguments\n");
        exit_code = 1;
    }
    error(db, NULL, NULL);
    ec_void(db);
    gc_void(db);
    if (db->stdin_dup != -1)
        close(db->stdin_dup);
    if (db->stdout_dup != -1)
        close(db->stdout_dup);
    exit(exit_code);
}