#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

void get_dir(t_db *db, char **store)
{
    t_env_list *home;
    t_env_list *curr_dir;
    char *prefix;
    int i;

    prefix = NULL;
    home = get_env_node(db->env_list, "HOME");
    curr_dir = get_env_node(db->env_list, "PWD");
    if (!home || !curr_dir)
        return ;
    i = 0;
    while (home->val[i] && curr_dir->val[i]
        && (home->val[i] == curr_dir->val[i]))
        i++;
    if (i > 1)
       prefix = "~";
    *store = ft_strjoin_ec(db, prefix, curr_dir->val + i);
}

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
    // char *prompt;

    // char *tmp;
    (void)db,
    // get_dir(db, &tmp);
    signal(SIGINT, handle_sigint);
    *line = readline("$ ");
    // handle ctrl + c later
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

    key = NULL;
    val = NULL;
    bool good = false;
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
    db->heredoc_counter = 0;
    db->ops = NULL;
    db->ops = NULL;
    db->root_node = NULL;
    db->error = false;
    db->curr_type = INVALID;
    db->input_fd = STDIN_FILENO;
    db->output_fd = STDOUT_FILENO;
}
void quit_signal_handler(int signal) {
    (void)signal;
}

int     main(int    ac, char    *av[],  char    *env[])
{
    t_db    db;
    char    *line;
    char    *tmp;
    int     ret;

    line = NULL;
    init_db(&db, ac, av, env);
    while (true)
    {
        db_reset(&db);
        signal(SIGQUIT, quit_signal_handler);
        ret = handle_prompt(&db, &line);
        signal(SIGQUIT, quit_signal_handler);
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
        wildcard(&db, line);
        exit(1);
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
