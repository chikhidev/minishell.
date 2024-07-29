#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

int handle_prompt(t_db *db, char **line)
{
    (void) db;
    printf(MAGENTA);
    *line = readline(GREEN"$> "RESET);
    printf(RESET);
    // handle ctrl + c later 
    if (!*line) return 0 ; // continue the loop
    if (*line[0] != '\0') add_history(*line);
    return SUCCESS ; // nothing
}

t_env_list *set_env_lst(t_db *db, char *env[]) {
    t_env_list *env_list = NULL;
    t_env_list *new_node = NULL;
    int i = 0;
    char *key;
    char *val;

    key = NULL;
    val = NULL;
    BOOL good = FALSE;
    while (env && env[i])
    {
        good = fill_key_val(db, env[i], &key, &val);
        if (!good)
            return NULL;
        new_node = new_env_node(db, key, val);
        if (!new_node) {
            error(db, NULL, "Malloc failed");
            return NULL; // Ensure we return if malloc fails.
        }
        // printf("%s[%s]", key, val);
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
    char            *key;
    char            *val;
    BOOL            good;
    exp_list = NULL;
    key = NULL;
    val = NULL;
    i = 0;
    while (env && env[i])
    {
        good = fill_key_val(db, env[i], &key, &val);
        if (!good)
            return (NULL);
        new_node = new_exp_node(db, key, val); // check malloc
        new_node->next = NULL;
        if (ft_strncmp(key, "_", ft_strlen(key)) == 0)
            new_node->visible = false;
        push_exp_sort(&exp_list, new_node);
        i++;
    }
    return exp_list;
}


void    init_db(t_db *db, int ac, char *av[], char *env[])
{
    int i;

    (void) ac;
    (void) av;
    db->debug = FALSE;
    db->gc = NULL;
    db->ec = NULL;
    db->here_docs = NULL;
    db->error = FALSE;
    db->env = env;
    db->env_list = set_env_lst(db, env);
    db->exp_list = set_exp_lst(db, env);
    i = 0;
    while (i < 6)
    {
        db->op_counter[i] = 0;
        i++;
    }
}

void db_reset(t_db *db)
{
    db->heredoc_counter = 0;
    db->ops = NULL;
    db->root_node = NULL;
    db->error = FALSE;
    db->last_signal = 0;
    db->curr_type = INVALID;
    db->input_fd = STDIN_FILENO;
    db->output_fd = STDOUT_FILENO;
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
        db_reset(&db);
        ret = handle_prompt(&db, &line);
        if (ret == 0) continue ;
        tmp = gc_malloc(&db, ft_strlen(line) + 1);
        if (!tmp) 
        {
            free(line);
            ec_void(&db);
            return !error(&db, NULL, "malloc failed");
        }
        ft_strlcpy(tmp, line, ft_strlen(line) + 1);
        free(line);
        line = tmp;
        if (parser(&db, line) == FAILURE)
            continue ;
        if (exec(&db, db.root_node) == FAILURE)
            continue ;
        gc_void(&db);
    }
    ec_void(&db);
    gc_void(&db);
    return (SUCCESS);
}