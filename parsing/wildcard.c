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

char	*strdup(const char *s1)
{
	const char	*clone;
	int			len;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	clone = malloc(sizeof(char) * (len + 1));
	if (!clone)
		return (NULL);
	ft_memcpy((void *)clone, s1, len + 1);
	return ((char *)clone);
}


char	*strjoin(char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		return (strdup(s2));
	else if (s1 && !s2)
		return (strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	return (res);
}
void    get_dir_files(  char    *path)
{
    DIR *curr_dir;
    struct dirent *entry;
    curr_dir = opendir(path);
    do
    {
        if (!curr_dir)
            return;
        entry = readdir(curr_dir);
        if (!entry)
            return;
        if (entry->d_type == DIR_ && !starts_with(entry->d_name, "."))
        {
            printf("%s \n\n", entry->d_name);
            path = strjoin(path, "/");
            get_dir_files(strjoin(path, entry->d_name));
            printf("\n");
        }
        else if (!starts_with(entry->d_name, "."))
        {
            printf("%s ", entry->d_name);
        }
    }
    while (entry);
}

char    *wildcard(t_db *db, char *line)
{
    (void)db;
    int i;

    i = 0;
    char   *cwd = "/nfs/homes/sgouzi/Desktop/minishell/parent";
    while (line[i])
    {
        if (line[i] == '*' && !is_inside_quotes_line(line, i))
        {
            get_dir_files(cwd);
        }
        i++;
    }
    return NULL;
}

