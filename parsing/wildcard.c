#include "../includes/exec.h"
#include "../includes/parsing.h"
#include "../includes/main.h"
#include "../includes/string.h"


#define FILE_ 8
#define DIR_ 4


/*
    * recursively read the curr dir
*/

int handle_wildcard(t_db *db, char  **result)
{
    DIR *curr_dir;
    struct dirent *entry;
    curr_dir = opendir(".");
    if (curr_dir)
    {
        entry = readdir(curr_dir);
        while (entry)
        {
            if (!starts_with(entry->d_name, "."))
                result = append_word(db, result, entry->d_name);
            entry = readdir(curr_dir);
        }
    }
    return (SUCCESS);

}


char    *wrap_with_signle_quote(t_db *db, char    *name)
{
    char    *new_name;

    new_name = NULL;
    new_name = ft_strjoin(db, new_name, "'");
    new_name = ft_strjoin(db, new_name, name);
    new_name = ft_strjoin(db, new_name, "'");

    return (new_name);
}

char    *get_dir_files(t_db *db, char    *path)
{
    char    *files;
    (void)db;
    DIR *curr_dir;
    struct dirent *entry;
    t_str_lst    *str_lst;
    t_str_lst    *new_str;

    files = NULL;
    str_lst = NULL;
    curr_dir = opendir(path);

    entry = readdir(curr_dir);
    if (entry)
    {
        files = ft_strjoin(db, files, entry->d_name);
        new_str = new_str_node(db, files);
        push_str_back(&str_lst, new_str);
    }
    while (entry)
    {
        entry = readdir(curr_dir);
        if (!entry)
            break;
        files = ft_strjoin(db, files, " ");
        files = ft_strjoin(db, files, entry->d_name);
        new_str = new_str_node(db, files);
        push_str_back(&str_lst, new_str);
    }
    return files;
}

char    *wildcard(t_db *db, char *line)
{
    (void)db;
    int i;
    char    *before_astress;
    char    *afterr_astress;
    char    *files;
    char    *new_line;
    char   *cwd = ".";
    i = 0;
    while (line[i])
    {
        if (line[i] == '*' && !is_inside_quotes_line(line, i))
        {
            before_astress = ft_substr(db, line, 0, i);
            files = get_dir_files(db, cwd);
            afterr_astress = ft_substr(db, line, i + 2, ft_strlen(line + i));
            new_line = ft_strjoin(db, before_astress, files);
            new_line = ft_strjoin(db, new_line, afterr_astress);
            printf("%s\n", new_line);
        }
        i++;
    }
    return NULL;
}

