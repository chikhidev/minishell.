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

int get_key_length(char *arg, bool  *append)
{
    int i;

    i = 0;
    while (arg[i])
    {
        if (arg[i] == '+')
        {
            *append = TRUE;
            return (i);
        }
        if (arg[i] == '=' || arg[i] == '\0')
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
        if (arg[i] == '\0')
            return len;
        i++;
        len++;
    }
    return (len);
}

char *get_key_from_arg(char *arg,int  *k_len, bool  *append)
{
    char    *key;

    *k_len = get_key_length(arg, append);
    key = malloc((*k_len + 1) * sizeof(char));
    if (!key)
        return FALSE;
    ft_strlcpy(key, arg, *k_len + 1);
    return (key);
}

char    *get_val_from_arg(char  *arg,   int *v_len, int k_len, bool append)
{
    char    *val;
    int     offset;

    offset = 0;
    if (append)
        offset = 1;
    *v_len = get_val_length(arg, k_len + 1 + offset);
    val = malloc((*v_len + 1) * sizeof(char));
    ft_strlcpy(val, &arg[k_len + offset + 1], *v_len + 1);
    val = whithout_quotes(val); // frees old val
    return val;
}

void    affect_node_val(t_exp_list  *node,  bool    append, char    *val)
{
    char    *joined;

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
        }
    }
}

bool handle_export_args(t_db    *db,    char    *args[])
{
    int i;
    char    *key;
    char    *val;
    int      k_len;
    int      v_len;
    t_exp_list  *exp_node;
    // t_env_list  *env_node;
    bool    good;
    bool    append;

    good = TRUE;
    append = FALSE;
    i = 1;
    while (args[i])
    {

        val = NULL;
        key = get_key_from_arg(args[i], &k_len, &append);
        exp_node = get_exp_node(db->exp_list, key); 
        // env_node = get_env_node(db->env_list)
        if (!good_export_var(key) || k_len < 1)
        {
            printf("export: `%s': not a valid identifier", args[i]);
            return FALSE;
        }
        else if (args[i][k_len] == '=' || args[i][k_len] == '+')
        {
            val = get_val_from_arg(args[i], &v_len, k_len, append);
            if (exp_node)
                affect_node_val(exp_node, append, val);
            else
            {
                exp_node = new_exp_node(db, key, val);
                push_exp_back(&db->exp_list, exp_node);
            }
        }
        else if (args[i][k_len] == '\0' || args[i][k_len] == ' ')
        {
            if (!exp_node)
            {
                exp_node = new_exp_node(db, key, val);
                push_exp_back(&db->exp_list, exp_node);
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