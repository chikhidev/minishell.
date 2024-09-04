#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"
#include "string.h"

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

    sa.sa_flags = 0;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);

    tmp = NULL;
    if (db->last_status == 0)
    {
        tmp = ft_strjoin(db, "\001" GREEN "\002>_ \001" RESET "\002", "$ ");
    }
    else
    {
        tmp = ft_strjoin(db, "\001" RED "\002>_ \001" RESET "\002", "$ ");
    }

    *line = readline(tmp);
    if (!*line)
    {
        dprintf(2, "exit\n");
        return FAILURE;
    }

    tmp = ft_strdup(db, *line);
    free(*line);
    *line = tmp;

    if (ft_strlen(*line) > 0)
        add_history(*line);

    return SUCCESS ; /*nothing*/
}

t_env_list *set_default_env(t_db *db)
{
    t_env_list  *new;

    db->static_path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
    new = new_env_node(db, "PWD", getcwd(NULL, 0));
    push_env_back(&db->env_list, new);
    new = new_env_node(db, "OSH", getcwd(NULL, 0));
    push_env_back(&db->env_list, new);
    new = new_env_node(db, "SHLVL", "0");
    push_env_back(&db->env_list, new);
    new = new_env_node(db, "_", "/usr/bin/env");
    push_env_back(&db->env_list, new);
    return (db->env_list);
}

t_exp_list *set_default_exp(t_db *db)
{
    t_exp_list  *new;

    new = new_exp_node(db, "PWD", getcwd(NULL, 0));
    push_exp_back(&db->exp_list, new);
    new = new_exp_node(db, "OSH", getcwd(NULL, 0));
    push_exp_back(&db->exp_list, new);
    new = new_exp_node(db, "SHLVL", "0");
    push_exp_back(&db->exp_list, new);
    new = new_exp_node(db, "_", "/usr/bin/exp");
    push_exp_back(&db->exp_list, new);
    return (db->exp_list);
}

void handle_shell_level(t_db *db, char *key, char **val)
{
    int shlvl;
    if (ft_strcmp(key, "SHLVL") == 0)
    {
        shlvl = ft_atoi((*val)) + 1;
        if (shlvl >= 1000)
            shlvl = 1;
        (*val) = ft_itoa_ec(db, shlvl);
    }
}

t_env_list *set_env_lst(t_db *db, char *env[]) {
    t_env_list *env_list = NULL;
    t_env_list *new_node = NULL;
    int i = 0;
    char *key;
    char *val;

    key = NULL;
    val = NULL;
    if (env == NULL || !env[0])
       return (set_default_env(db));
   
    while (env && env[i])
    {
        fill_key_val(db, env[i], &key, &val);
        handle_shell_level(db, key, &val);
        new_node = new_env_node(db, key, val);
        push_env_back(&env_list, new_node);
        i++;
    }
    return env_list;
}

void handle_pwd(t_db *db)
{
    t_exp_list *exp;
    t_exp_list *pwd_exp;
    bool    pwd_exist;

    pwd_exist = false;
    exp = db->exp_list;
    while (exp)
    {
        if (ft_strcmp(exp->key, "PWD"))
        {
            pwd_exist = true;
            break;
        }
        exp = exp->next;
    }
    if (!pwd_exist)
    {
        pwd_exp = new_exp_node(db, "PWD", getcwd(NULL, 0));
        push_exp_sort(&db->exp_list, pwd_exp);
    }
}

t_exp_list    *set_exp_lst(t_db   *db, char   *env[])
{
    t_exp_list      *exp_list;
    t_exp_list      *new_node;
    int             i;
    char            *key;
    char            *val;
    exp_list = NULL;
    key = NULL;
    val = NULL;
    i = 0;
    if (env == NULL || !env[0])
        return (set_default_exp(db));
    while (env && env[i])
    {
        fill_key_val(db, env[i], &key, &val);
        handle_shell_level(db, key, &val);
        new_node = new_exp_node(db, key, val);
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
    db->last_status = 0;
    db->debug = false;
    db->gc = NULL;
    db->ec = NULL;
    db->error = false;
    db->env = env;
    db->env_list = NULL;
    db->exp_list = NULL;
    db->static_path = NULL;
    db->env_list = set_env_lst(db, env);
    db->exp_list = set_exp_lst(db, env);
    db->pid = NULL;
    db->fd = NULL;
    db->stdin_dup = -1;
    db->stdout_dup = -1;
}

void db_reset(t_db *db)
{
    db->exec_line = true;
    db->heredoc_counter = 0;
    db->root_node = NULL;
    db->error = false;
    db->curr_type = INVALID;
    db->input_fd = STDIN_FILENO;
    db->output_fd = STDOUT_FILENO;
    db->fd = NULL;
    pid_void(db);
}

int main(int ac, char *av[],  char *env[])
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

        sa.sa_handler = SIG_IGN;
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGQUIT, &sa, NULL);
        
        if (ret == FAILURE)
            break ;
        if (ret == 0)
            continue ;
        tmp = gc_malloc(&db, ft_strlen(line) + 1);

        ft_strlcpy(tmp, line, ft_strlen(line) + 1);
        
        if (parser(&db, line) == SUCCESS)
        {
            exec(&db, db.root_node);
        }
        fd_void(&db);
        gc_void(&db);
        pid_void(&db);
    }
    fd_void(&db);
    ec_void(&db);
    gc_void(&db);

    close(0);
    close(1);
    close(2);
    
    return (0);
}
