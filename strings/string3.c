#include "main.h"
#include "parsing.h"

int count_line_args(char *line, t_quote *quotes, int len)
{
    int i;
    int reminder;
    int count;

    i = 0;
    count = 0;
    skip_spaces(line, &i);
    while (i < len)
    {
        reminder = i;
        while (line[i] && !is_whitespace(line[i]) && !is_inside_quotes_list(quotes, i))
            i++;
        if (reminder != i)
            count++;
        skip_spaces(line, &i);
    }
    return count;
}

int append_split(char **splitted, char *string)
{
    int i;

    if (!string)
        return FAILURE;

    i = 0;
    while (splitted[i])
        i++;
    splitted[i] = string;

    return SUCCESS;
}

char *sub(t_db *db, char *line, int i, int j)
{
    char *res;

    res = gc_malloc(db, sizeof(char) * (j - i + 1));
    if (!res)
        return NULL;
    ft_strlcpy(res, line + i, j - i + 1);

    return res;
}

/**
 * @details This function checks if the string is all whitespaces, pay attention that it frees the original string
 */
char *whithout_quotes_free_db(t_db *db, char *line)
{
    int i;
    char *res;
    int size;

    i = 1;
    size = ft_strlen(line);

    if (!(
        (line[0] == SGL_QUOTE && line[size - 1] == SGL_QUOTE)
        || (line[0] == DBL_QUOTE && line[size - 1] == DBL_QUOTE)
    ))
        return line;

    size -= 2;
    res = gc_malloc(db, sizeof(char) * (size + 1));
    if (!res)
        return NULL;
    ft_strlcpy(res, line + i, size + 1);
    gc_free(db, line);
    return res;
}

short inside_quot_v2(short single_opened, short  double_opened)
{
    if (single_opened && !double_opened)
        return true;
    else if (double_opened && !single_opened)
        return true;
    else 
        return false;
}

int get_str_size_unquoted( char  *str)
{
    int i;
    bool   single_opened;
    bool   double_opened;
    int len;
    single_opened = false;
    double_opened = false;
    len = 0;
    i = 0;
    while (str[i])
    {
        // toggling
        if (str[i] == SGL_QUOTE && !double_opened)
        {
            if (single_opened)
                single_opened = false;
            else
                single_opened = true;
            i++;
            continue;
        }
        if (str[i] == DBL_QUOTE && !single_opened)
        {
            if (double_opened)
                double_opened = false;
            else
                double_opened = true;
            i++;
            continue;
        }
        len++;
        i++;
    }
    return len;
}

bool is_inside_quotes_line(char *line, int index)
{
    bool in_single_quote = false;
    bool in_double_quote = false;
    int i = 0;
    while(i <= index)
    {
        if (line[i] == SGL_QUOTE)
            in_single_quote = !in_single_quote;
        else if (line[i] == DOBLQUOTE)
            in_double_quote = !in_double_quote;
        i++;
    }
    return (in_single_quote || in_double_quote);
}

char *without_quotes(t_db *db, char *str, t_quote *q)
{
    int start;
    int end;
    int increment;

    start = 0;
    if (!q)
    {
        increment = ((
            (str[0] == SGL_QUOTE || str[0] == DBL_QUOTE)
        ) && (
            str[ft_strlen(str) - 1] == SGL_QUOTE || str[ft_strlen(str) - 1] == DBL_QUOTE
        ));
        start += increment;
        end = ft_strlen(str) - increment;
        return ft_substr(db, str, start, end - start);
    }
    while (str[start] && !is_inside_quotes_list(q, start))
        start++;
    end = start;
    while (str[end] && is_inside_quotes_list(q, end))
        end++;
    char *res = ft_substr(db, str, start, end - start);
    return res;
}

// char *whithout_quotes(t_db *db, char *str)
// {
//     int i;
//     char *res;
//     // int size;
//     bool   single_opened;
//     bool   double_opened;

//     single_opened = false;
//     double_opened = false;
//     res = ft_strdup(db, "");
//     i = 0;
//     while (str[i])
//     {
//         // toggling
//         if (str[i] == SGL_QUOTE && !double_opened)
//         {
//             single_opened = !single_opened;
//             i++;
//             continue;
//         }
//         if (str[i] == DBL_QUOTE && !single_opened)
//         {
//             double_opened = !double_opened;
//             i++;
//             continue;
//         }
//         res = ft_strjoin_char(db, res, str[i]);
//         i++;
//     }
//     return res;
// }

char *whithout_quotes_ec(t_db  *db,  char *str)
{
    int i;
    char *res;
    // int size;
    bool   single_opened;
    bool   double_opened;

    single_opened = false;
    double_opened = false;
    res = ft_strdup_ec(db, "");
    i = 0;
    while (str[i])
    {
        // toggling
        if (str[i] == SGL_QUOTE && !double_opened)
        {
            if (single_opened)
                single_opened = false;
            else
                single_opened = true;
            i++;
            continue;
        }
        if (str[i] == DBL_QUOTE && !single_opened)
        {
            if (double_opened)
                double_opened = false;
            else
                double_opened = true;
            i++;
            continue;
        }
        res = ft_strjoin_char_ec(db,    res, str[i]);
        i++;
    }
    return res;
}

bool contains(char  *str, char    *sub)
{
    int i;
    int j;

    i = 0;
    while (str[i])
    {
        j = 0;
        while (str[i + j] == sub[j] && str[i + j] && sub[j])
            j++;
        if (!sub[j])
            return true;
        i++;
    }
    return false;
}

char	*ft_strjoin_char(t_db *db, char *s1, char c)
{
	int		len1;
	int		len2;
	char	*res;

	if (!s1)
    {
        if (c == '\0')
        {
            res = gc_malloc(db, 1 * sizeof(char));
            res[0] = '\0';
        }    
        else
        {
            res = gc_malloc(db, 2 * sizeof(char));
            res[0] = c;
            res[1] = '\0';
        }
        return (res);
    }
	len1 = ft_strlen(s1);
    len2 = 1;
	if (c == '\0')
        len2 = 0;
	res = gc_malloc(db, sizeof(char) * (len1 + len2 + 1));
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, &c, len2);
    gc_free(db, s1);
	res[len1 + len2] = '\0';
	return (res);
}

char	*ft_strjoin_char_ec(t_db    *db,    char *s1, char c)
{
	int		len1;
	int		len2;
	char	*res;

	if (!s1)
    {
        if (c == '\0')
        {
            res = ec_malloc(db, 1 * sizeof(char));
            res[0] = '\0';
        }    
        else
        {
            res = ec_malloc(db, 2 * sizeof(char));
            res[0] = c;
            res[1] = '\0';
        }
        return (res);
    }
	len1 = ft_strlen(s1);
    len2 = 1;
	if (c == '\0')
        len2 = 0;
	res = ec_malloc(db, sizeof(char) * (len1 + len2 + 1));
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, &c, len2);
    gc_free(db, s1);
	res[len1 + len2] = '\0';
	return (res);
}