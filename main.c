#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

void handle_sigint(int signum)
{
    (void)signum;

    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}


int handle_prompt(t_db *db, char **line)
{    
    (void)db;
    struct sigaction sa;
    char *tmp;

    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);

    tmp = NULL;
    if (db->last_signal == 0)
    {
        tmp = ft_strjoin(db, "\001" GREEN "\002>_\001" RESET "\002", "$ ");
    }
    else
    {
        tmp = ft_strjoin(db, "\001" RED "\002>_\001" RESET "\002", "$ ");
    }

    *line = readline(tmp);
    gc_free(db, tmp);
    if (!*line) return FAILURE; /*stop the loop*/
    if (*line[0] != '\0') add_history(*line);
    return SUCCESS ; /*nothing*/
}

t_env_list *set_env_lst(t_db *db, char *env[]) {
    t_env_list *env_list = NULL;
    t_env_list *new_node = NULL;
    int i = 0;
    char *key;
    char *val;
    bool good = false;

    key = NULL;
    val = NULL;
    while (env && env[i])
    {
        good = fill_key_val(db, env[i], &key, &val);
        if (!good)
            return NULL ;
        new_node = new_env_node(db, key, val);
        if (!new_node) {
            error(db, NULL, "Malloc failed6");
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
    bool            good;
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
    (void) ac;
    (void) av;
    db->last_signal = 0;
    db->debug = false;
    db->gc = NULL;
    db->ec = NULL;
    db->error = false;
    db->env = env;
    db->env_list = set_env_lst(db, env);
    db->exp_list = set_exp_lst(db, env);
    db->ip = NULL;
    db->stdin_dup = -1;
    db->stdout_dup = -1;
}

void db_reset(t_db *db)
{
    db->scope = false;
    db->exec_line = true;
    db->heredoc_counter = 0;
    db->ops = NULL;
    db->ops = NULL;
    db->root_node = NULL;
    db->error = false;
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
    struct sigaction sa;


    ft_bzero(&sa, sizeof(struct sigaction));
    line = NULL;
    init_db(&db, ac, av, env);
    while (true)
    {
        db_reset(&db);
        ret = handle_prompt(&db, &line);

        // signal handlig to ignore SIGINT and SIGQUIT
        sa.sa_handler = SIG_IGN;
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGQUIT, &sa, NULL);
        // sigaction(SIGQUIT, &sa, NULL);


        if (ret == FAILURE)
            break ;
        if (ret == 0)
            continue ;
        tmp = gc_malloc(&db, ft_strlen(line) + 1);
        if (!tmp)
        {
            free(line);
            ec_void(&db);
            return !error(&db, NULL, "malloc failed");
        }
        ft_strlcpy(tmp, line, ft_strlen(line) + 1);
        // wildcard(&db, line);
        // exit(1);
        free(line);
        line = tmp;
        if (parser(&db, line) == FAILURE)
            continue ;
        if (exec(&db, db.root_node, -1) == FAILURE)
            continue ;

        gc_void(&db);
        ip_void(&db);
    }
    ec_void(&db);
    gc_void(&db);
    ip_void(&db);
    close(0);
    close(1);
    close(2);
    return (SUCCESS);
}
