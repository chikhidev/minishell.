#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

char    *line_promte()
{
    char    *user;
    char    *tmp;

    user = getenv("USER");
    if (user)
        user = ft_strdup(user);
    else
        user = ft_strdup("shell");
    if (!user)
        return (NULL);
    tmp = ft_strjoin(GREEN"Lowa@"RESET, user);
    free(user);
    if (!tmp)
        return (ft_strdup(GREEN"Lowa@shell~"RESET));
    user = ft_strjoin(tmp, ORANGE"~ "RESET);
    free(tmp);
    if (!user)
        return (ft_strdup(GREEN"Lowa@shell~"RESET));
    return (user);
}

int handle_prompt(char **line)
{
    char    *promte;

    promte = line_promte();
    *line = readline(promte);
    free(promte);
    // handle ctrl + c later 
    if (!*line) return 0 ; // continue the loop
    if (ft_strncmp(*line, "exit", 4) == 0) return -1 ; // break the loop
    if (*line[0] != '\0') add_history(*line);
    return 1 ; // nothing
}

t_env_list    *set_env_lst(t_db   *db, char   *env[])
{
    t_env_list      *env_list;
    t_env_list      *new_node;
    int             i;

    env_list = NULL;
    i = 0;
    while (env[i])
    {
        new_node = new_env_node(db, env[i]);
        // if (!new_node)
        //     (gc_void(db), exit(1));
        push_env_back(&env_list, new_node);
        i++;
    }
    return env_list;
}

t_exp_list    *set_exp_lst(t_db   *db, char   *env[])
{
    t_exp_list      *exp_list;
    t_exp_list      *new_node;
    int             i;
    int             len;
    char            *key;
    char            *val;

    exp_list = NULL;
    i = 0;
    while (env[i])
    {
        // if (!new_node)
        //     (gc_void(db), exit(1));
        len = length_til(env[i], '=');
        key = malloc(len + 1); // check malloc
        ft_strlcpy(key, env[i], len + 1);
        val = malloc(ft_strlen(env[i]) - len + 1); // check malloc
        ft_strlcpy(val, env[i] + len, ft_strlen(env[i]) - len + 1);
        new_node = new_exp_node(db, key, val); // check malloc
        new_node->next = NULL;
        if (ft_strncmp(key, "_", ft_strlen(key)) == 0)
            new_node->visible = false;
        push_exp_back(&exp_list, new_node);
        i++;
    }
    return exp_list;
}


void    init_db(t_db *db, int ac, char *av[], char *env[])
{
    int i;

    db->debug = FALSE;
    db->gc = NULL;
    db->root_node = NULL;
    db->ops = NULL;
    db->here_docs = NULL;
    db->error = FALSE;
    db->env = env;
    db->last_signal = 0;
    db->curr_type = INVALID;
    db->input_fd = INVALID;
    db->output_fd = INVALID;
    db->env_list = set_env_lst(db, env);
    // printf("env_list\n");
    db->exp_list = set_exp_lst(db, env);
    // print exp_list
    (void) ac;
    (void) av;
    i = 0;
    while (i < 6)
    {
        db->op_counter[i] = 0;
        i++;
    }
}

void free_environment(t_db  *db)
{
    t_env_list  *next_env;
    t_env_list  *curr_env;
    t_exp_list  *next_exp;
    t_exp_list  *curr_exp;

    curr_env = db->env_list;
    while (curr_env)
    {
        next_env = curr_env->next;
        free(curr_env);
        curr_env = next_env;
    }
    curr_exp = db->exp_list;
    printf("exp_list\n");
    while (curr_exp)
    {
        next_exp = curr_exp->next;
        free(curr_exp->key);
        free(curr_exp->val);
        free(curr_exp);
        curr_exp = next_exp;
    }
}

int     main(int    ac, char    *av[],  char    *env[])
{
    t_db    db;
    char    *line;
    char    *tmp;
    int     ret;

    line = NULL;
    init_db(&db, ac, av, env);
    while (TRUE)
    {
        printf(MAGENTA);
        ret = handle_prompt(&line);
        printf(RESET);
        if (ret == -1) break ;
        else if (ret == 0) continue ;

        tmp = gc_malloc(&db, ft_strlen(line) + 1);
        if (!tmp) return !error(&db, NULL, "malloc failed");
        ft_strlcpy(tmp, line, ft_strlen(line) + 1);
        free(line);
        line = tmp;

        /* parse the line ----------------------------------*/
        if (parser(&db, line) == FAILURE)
            continue ;
        if (exec(&db, db.root_node) == FAILURE)
            continue ;
        /* execute the cmds --------------------------------*/
        // env_list = db.env_list;
        // while (env_list)
        // {
        //     printf("%s\n", env_list->data);
        //     env_list = env_list->next;
        // }
        gc_void(&db);
    }
    free_environment(&db);
    gc_void(&db);
    return (SUCCESS);
}
