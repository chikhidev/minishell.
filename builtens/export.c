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
        if (ft_strcmp(curr->key, "_") != 0)
        {
            printf("declare -x %s", curr->key);
            if (curr->val)
                printf("=\"%s\"", curr->val);
            printf("\n");
        }
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
    char    *val;
    int      k_len;
    int      v_len;
    char    *joined;
    t_exp_list  *node;
    bool    good;
    bool    append;

    good = TRUE;
    i = 1;
    while (args[i])
    {
        printf("arg -> %s\n", args[i]);
        k_len = get_key_length(args[i]);
        key = malloc((k_len + 1) * sizeof(char));
        if (!key)
            return FALSE;
        ft_strlcpy(key, args[i], k_len + 1);
        val = NULL;
        node = get_exp_node(db->exp_list, key);
        append = FALSE;
        if (!good_export_var(key) || k_len < 1)
        {
            printf("export: `%s': not a valid identifier", args[i]);
            return FALSE;
        }
        else if (args[i][k_len] == '=')
        {
            if (k_len > 0 && args[i][k_len - 1] == '+')
                append = true;
            v_len = get_val_length(args[i], k_len + 1);
            val = malloc((v_len + 1) * sizeof(char));
            ft_strlcpy(val, &args[i][k_len + 1], v_len + 1);
            val = whithout_quotes(val); // frees old val
            if (node)
            {
                if (append)
                {
                    joined = ft_strjoin(node->val, val);
                    free(node->val);
                    free(val);
                    node->val = joined;
                }
                else
                {
                    free(node->val);
                    node->val = val;
                    good = true;
                }
            }
            else
            {
                node = new_exp_node(db, key, val);
                push_exp_back(&db->exp_list, node);
            }
        }
        else if (args[i][k_len] == '\0' || args[i][k_len] == ' ')
        {
            if (!node)
            {
                node = new_exp_node(db, key, val);
                push_exp_back(&db->exp_list, node);
            }
        }
        else
        {
            printf("export: `%s': not a valid identifier", args[i]);
            return FALSE;
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