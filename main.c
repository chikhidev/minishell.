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
    t_exp_list *old_pwd_exp;
    t_env_list *pwd_env;
    bool    pwd_exist;
    bool    old_pwd_exist;

    pwd_exist = false;
    old_pwd_exist = false;
    exp = db->exp_list;
    while (exp)
    {
        if (ft_strcmp(exp->key, "PWD") == 0)
            pwd_exist = true;
        if (ft_strcmp(exp->key, "OLDPWD") == 0)
            old_pwd_exist = true;
        exp = exp->next;
    }
    if (!pwd_exist)
    {
        pwd_exp = new_exp_node(db, ft_strdup_ec(db, "PWD"), ft_strdup_ec(db, getcwd(NULL, 0)));
        push_exp_sort(&db->exp_list, pwd_exp);
        pwd_env = new_env_node(db, ft_strdup_ec(db, "PWD"), ft_strdup_ec(db, getcwd(NULL, 0)));
        push_env_back(&db->env_list, pwd_env);
    }
    if (!old_pwd_exist)
    {
        old_pwd_exp = new_exp_node(db, ft_strdup_ec(db, "OLDPWD"), NULL);
        push_exp_sort(&db->exp_list, old_pwd_exp);
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
    handle_pwd(db);
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

bool match(const char *string, const char *pattern)
{
    // If both pattern and string are empty, they match
    if (*pattern == '\0')
        return *string == '\0';

    // If pattern is empty but string is not, they don't match
    if (*string == '\0')
    {
        // Skip remaining '*' in the pattern
        while (*pattern == '*')
            pattern++;
        return *pattern == '\0';
    }

    // If pattern starts with '*'
    if (*pattern == '*')
    {
        // Move past the '*' and try matching the rest of the pattern
        while (*pattern == '*')
            pattern++;
        if (*pattern == '\0')
            return true;  // If '*' was the last character, it matches
        while (*string != '\0')
        {
            if (match(string, pattern))
                return true;
            string++;
        }
        return false;
    }

    // If pattern and string characters match
    if (*pattern == *string)
        return match(string + 1, pattern + 1);

    return false;
}

void handle_wildcard(t_db *db, char ***result, const char *pattern) {
    DIR *curr_dir;
    struct dirent *entry;
    (void) pattern;
    curr_dir = opendir("abc");
    if (curr_dir)
    {
        entry = readdir(curr_dir);
        while (entry)
        {
            if (match(entry->d_name, "a*")) {
                *result = append_word(db, *result, entry->d_name, false);
            }
            entry = readdir(curr_dir);
        }
        closedir(curr_dir);
    }
}

char **wildcard(t_db *db, const char *pattern)
{
    char **files;
    files = gc_malloc(db, sizeof(char *));
    files[0] = NULL;
    handle_wildcard(db, &files, pattern);
    int i = 0;
    while (files[i])
        printf("-> %s\n", files[i++]);
    return files;
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
