#include "../includes/exec.h"
#include "../includes/parsing.h"
#include "../includes/main.h"
#include "../includes/string.h"

bool    starts_with(char    *str,   char    *sub)
{
    return ft_strncmp(str, sub, ft_strlen(sub)) == 0;
}

#define FILE_ 8
#define DIR_ 4


/*
    * recursively read the curr dir
*/

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
    bool    first_time;

    first_time = true;
    files = NULL;
    curr_dir = opendir(path);

    entry = readdir(curr_dir);
    if (entry)
    {
        if (all_whitespaces(entry->d_name, 0, ft_strlen(entry->d_name)))
        {
            files = ft_strjoin(db, files, "'");
            files = ft_strjoin(db, files, entry->d_name);
            files = ft_strjoin(db, files, "'");
        }
        else
            files = ft_strjoin(db, files, entry->d_name);
    }
    while (entry)
    {
        entry = readdir(curr_dir);
        if (!entry)
            break;
        files = ft_strjoin(db, files, " ");
        if (all_whitespaces(entry->d_name, 0, ft_strlen(entry->d_name)))
        {
            files = ft_strjoin(db, files, "'");
            files = ft_strjoin(db, files, entry->d_name);
            files = ft_strjoin(db, files, "'");
        }
        else
            files = ft_strjoin(db, files, entry->d_name);
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

