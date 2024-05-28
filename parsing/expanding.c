#include "../includes/main.h"
#include "../includes/parsing.h"

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

char *ft_strncpy(char *dst, char *src, int n)
{
    int i;

    i = 0;
    while (src[i] && i < n)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}

char *get_env_name(t_db *db, char  *line, int idx)
{
    int var_len;
    char    *env_var_name;
    char    *expanded_line;
    int initial_idx;
    var_len = 0;
    initial_idx = idx;
    idx++;
    while (line[idx])
    {
        if (!line[idx] || is_whitespace(line[idx]))
            break;
        idx++;
        var_len++;
    }
    if (var_len == 0)
    {
        expanded_line = ft_strdup(line);
        if (!expanded_line)
            return (NULL);
    }
    env_var_name = gc_malloc(db, (var_len + 1) * sizeof(char));
    env_var_name = ft_strncpy(env_var_name, &line[initial_idx + 1], var_len);
    return env_var_name;

}

int skip_non_whitespaces(char  *line,   int *init_i)
{
    int i;

    i = *init_i;
    while (line[i] && !is_whitespace(line[i]))
        i++;
    *init_i = i;
    return (SUCCESS);
}

int add_token(t_tokens **tokens, char *token)
{
    t_tokens *new;
    t_tokens *tmp;

    new = malloc(sizeof(t_tokens));
    if (!new)
        return (FAILURE);
    new->token = token;
    new->next = NULL;
    if (!*tokens)
    {
        *tokens = new;
        return (SUCCESS);
    }
    tmp = *tokens;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    return (SUCCESS);
}

int del_tokens(t_tokens **tokens)
{
    t_tokens *tmp;
    t_tokens *next;

    tmp = *tokens;
    while (tmp)
    {
        next = tmp->next;
        // free(tmp->token);
        free(tmp);
        tmp = next;
    }
    *tokens = NULL;
    return (SUCCESS);
}

char *get_env(t_db *db, char *name)
{
	int i;

	i = 0;
	while (db->env[i])
    {
		if (ft_strncmp(name, db->env[i], ft_strlen(name)) == 0 && db->env[i][ft_strlen(name)] == '=')
			return (db->env[i] + ft_strlen(name) + 1);
		i++;
    }
	return (NULL);
}

int inside_single_quote(t_db *db, int   i)
{
    t_quote *q;

    q = db->quotes;
    while (q)
    {
        if (q->start <= i && i >= q->end && q->type == SNGLQUOTE)
            return (1);
        q = q->next;
    }
    return (0);
}

int expand(t_db *db, char   *line)
{
    int i;
    char *env_var_name;
    char *env_var_value;
    t_tokens *tokens;

    tokens = NULL;
    i = 0;
    while (line[i])
    {
        if (line[i] == '$' && !is_whitespace(line[i + 1]))
        {
            if (!inside_single_quote(db, i))
            {
                env_var_name = get_env_name(db, line, i);
                if (env_var_name)
                {
                    env_var_value = get_env(db, env_var_name);
                    if (env_var_value)
                        add_token(&tokens, env_var_value);
                    else
                        add_token(&tokens, "$");
                }
            }
            else
                printf("no expand\n");
        }
        i++;
    }
    while (tokens)
    {
        printf("token: %s\n", tokens->token);
        tokens = tokens->next;
    }
    return (SUCCESS);
    
}