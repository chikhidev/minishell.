#include "main.h"
#include "parsing.h"
#include "string.h"

int show_export(t_db *db)
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
    return (SUCCESS);
}

int    has_special_char(char   *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '+' && str[i] != '=')
            return (true);
        i++;
    }
    return (false);
}

int    good_export_var(char    *var)
{
    if (var && (ft_isdigit(var[0])))
        return (false);
    else if (!ft_isalpha(var[0]) && var[0] != '_')
        return (false);
    else if (has_special_char(var))
        return (false);
    else
        return true;
    return true;
}

int get_key_length(char *arg, bool  *append)
{
    int i;

    i = 0;
    while (arg[i])
    {
        if (arg[i] == '+')
        {
            *append = true;
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

char *get_key_from_arg(t_db *db,    char *arg,int  *k_len, bool  *append)
{
    char    *key;

    *k_len = get_key_length(arg, append);
    key = ec_malloc(db, (*k_len + 1) * sizeof(char));
    if (!key)
        return false;
    ft_strlcpy(key, arg, *k_len + 1);
    return (key);
}

char    *get_val_from_arg(t_db  *db,    char  *arg,   int *v_len, int k_len, bool append)
{
    char    *val;
    int     offset;

    offset = 0;
    if (append)
        offset = 1;
    *v_len = get_val_length(arg, k_len + 1 + offset);
    val = ec_malloc(db, (*v_len + 1) * sizeof(char));
    if (!val)
        return false;
    ft_strlcpy(val, &arg[k_len + offset + 1], *v_len + 1);
    return val;
}

int    affect_exp_node_val(t_db *db, t_exp_list  *node,  bool    append, char    *val)
{
    char    *joined;

    if (node)
    {
        if (append)
        {
            joined = ft_strjoin_ec(db, node->val, val);
            if (!joined)
                return (false);
            node->val = joined;
        }
        else
        {
            node->val = val;
        }
    }
    return (SUCCESS);
}

int    affect_env_node_val(t_db *db, t_env_list  *node,  bool    append, char    *val)
{
    char    *joined;

    if (node)
    {
        if (append)
        {
            joined = ft_strjoin_ec(db, node->val, val);
            if (!joined)
                return (false);
            node->val = joined;
        }
        else
            node->val = val;
    }
    return (SUCCESS);
}

bool    fill_key_val(t_db   *db,    char  *arg,   char  **key,   char    **val)
{
    bool    append;
    int      k_len;
    int      v_len;

    v_len = 0;
    k_len = 0;
    append = false;
    *key = get_key_from_arg(db, arg, &k_len, &append);
    *val = get_val_from_arg(db, arg, &v_len, k_len, append);

    if (key && val)
        return true;
    return false;
}

int handle_export_args(t_db    *db,    char    *args[])
{
    int i;
    char    *key;
    char    *token;
    char    *val;
    int      k_len;
    int      v_len;
    t_exp_list  *exp_node;
    t_env_list  *env_node;
    bool    good;
    bool    append;

    good = true;
    append = false;
    i = 1;
    while (args[i])
    {
        token = whithout_quotes(db, args[i]);
        val = NULL;
        key = get_key_from_arg(db,  token, &k_len, &append);
        exp_node = get_exp_node(db->exp_list, key); 
        env_node = get_env_node(db->env_list, key);
        if ((!good_export_var(key) || k_len < 1) || (append && token[k_len + 1] != '='))
        {
            printf("export: `%s': not a valid identifier\n", args[i]);
            good = false;
        }
        else if (token[k_len] == '=' || token[k_len] == '+')
        {
            val = get_val_from_arg(db,  token, &v_len, k_len, append);
            if (exp_node)
                affect_exp_node_val(db, exp_node, append, val);
            else
            {
                key = ft_strdup_ec(db, key);
                val = ft_strdup_ec(db, val);
                exp_node = new_exp_node(db, key, val);
                push_exp_sort(&db->exp_list, exp_node);
            }
            if (env_node)
                affect_env_node_val(db, env_node, append, val);
            else
            {
                key = ft_strdup_ec(db, key);
                val = ft_strdup_ec(db, val);
                env_node = new_env_node(db, key, val);
                push_env_back(&db->env_list, env_node);
            }
        }
        else if (token[k_len] == '\0' || token[k_len] == ' ')
        {
            if (!exp_node)
            {
                exp_node = new_exp_node(db, key, val);
                push_exp_sort(&db->exp_list, exp_node);
            }
        }
        else
        {
            printf("export: `%s': not a valid identifier\n", args[i]);
            good = false;
        }
        i++;
    }
    return good;
}

int export(t_db    *db, char   *args[])
{
    int n_args;

    n_args = count_array_len(args);
    if (n_args == 1)
        return show_export(db);
    return handle_export_args(db, args);
}