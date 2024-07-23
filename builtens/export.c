#include "main.h"
#include "parsing.h"
#include "string.h"

bool show_export(t_db *db)
{
    // show export here;
    t_exp_list  *vars;
    t_exp_list  *curr;

    vars = db->exp_list;
    curr = vars;
    while (curr)
    {
        if (curr->visible)
            printf("declare -x %s=\"%s\"\n", curr->key, curr->val);
        curr = curr->next;
    }
    return true;
}

bool    has_special_char(char   *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return true;
        i++;
    }
    return (false);
}

bool    good_export_var(char    *var)
{
    // dont mind the $ cause we already expanded here
    if (var && (ft_isdigit(var[0])))
        return (false);
    else if (!ft_isalpha(var[0]) && var[0] != '_')
        return (false);
    // else if (has_special_char(var))
        // return (false);
    else
    {
        printf("-> %s\n", var);
        return true;
    }
    return true;
    // no special caractwrs
}

bool handle_export_args(t_db    *db,    char    *args[])
{
    int i;
    (void)db;
    t_exp_list  *new;
    char    *key;
    char    *val;
    int      len;
    i = 1;
    bool    good;

    good = TRUE;
    while (args[i])
    {
        if (!good_export_var(args[i]))
        {
            printf("export: `%s`: not a valid identifier\n", args[i]);
            good = FALSE;
        }
        else
        {
            if (contains(args[i], "="))
            {
                len = length_til(args[i], '=');
                key = malloc((len + 1) * sizeof(char));
                if (!key)
                    return FALSE;
                ft_strlcpy(key, args[i], len + 1);
                val = malloc(ft_strlen(args[i]) - len - 1 + 1); // -1 => '='   + 1  => '\0'
                ft_strlcpy(val, args[i] + len + 1, ft_strlen(args[i]) - len - 1 + 1);
                // printf("key[%d] -> %s\n", len + 1, key);
                // printf("val[%ld] -> %s\n", ft_strlen(args[i]) - len - 1 + 1, val);
                new = new_exp_node(db, key, val);
                if (!new)
                {
                    gc_void(db);
                    exit(1);
                }
                push_exp_back(&db->exp_list, new);
            }

        }
        i++;
    }
    return good;
}

bool export(t_db    *db, char   *args[])
{
    int n_args;

    n_args = count_array_len(args);
    if (n_args == 1)
        return show_export(db);
    return handle_export_args(db, args);
}