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
        printf("declare -x %s", curr->key);
        if (curr->val)
            printf("=\"%s\"", curr->val);
        printf("\n");
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
        if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '+' && str[i] != '=')
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
    else if (has_special_char(var))
        return (false);
    else
    {
        printf("-> %s\n", var);
        return true;
    }
    return true;
    // no special caractwrs
}

int get_key_length(char *arg)
{
    int i;

    i = 0;
    while (arg[i])
    {
        if (arg[i] == ' ' || arg[i] == '+' || arg[i] == '=' || arg[i] == '\0')
            return i;
        i++;
    }
    return (i);
}

int get_val_length(char *arg,   int start_idx)
{
    int i;
    int len;

    len = 0;
    i = start_idx;
    while (arg[i])
    {
        if (arg[i] == ' ' || arg[i] == '\0')
            return len;
        i++;
        len++;
    }
    return (len);
}

bool handle_export_args(t_db    *db,    char    *args[])
{
    int i;
    char    *key;
    char    *tmp;
    char    *val;
    int      k_len;
    int      v_len;
    t_exp_list  *new;
    bool    good;

    good = TRUE;
    i = 1;
    while (args[i])
    {
        k_len = get_key_length(args[i]);
        key = malloc((k_len + 1) * sizeof(char));
        if (!key)
            return FALSE;
        ft_strlcpy(key, args[i], k_len + 1);
        if (!good_export_var(key) || k_len < 1)
        {
            printf("export: `%s': not a valid identifier", args[i]);
            return FALSE;
        }
        else if (args[i][k_len] == '\0' || args[i][k_len] == ' ')
        {
            val = NULL;
        }
        else if (args[i][k_len] == '=')
        {
            v_len = get_val_length(args[i], k_len + 1);
            val = malloc((v_len + 1) * sizeof(char));
            CATCH_ONNULL(
                val, NULL
            )
            tmp = whithout_quotes(db, val);
            CATCH_ONNULL(
                tmp, NULL
            )
            free(val);
            val = tmp;
            ft_strlcpy(val, &args[i][k_len + 1], v_len + 1);
        }
        else
        {
            printf("export: `%s': not a valid identifier", args[i]);
            return FALSE;
        }
        new = new_exp_node(db, key, val);
        if (!new)
        {
            gc_void(db);
            exit(1);
        }
        push_exp_back(&db->exp_list, new);
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