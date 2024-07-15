# include "main.h"
# include "parsing.h"

void  skip_open_spaces(t_quote *quotes, char *line, int *i)
{
    while (line[*i] && is_whitespace(line[*i]) && !is_inside_quotes(quotes, *i))
        (*i)++;
}

static int	count_words(t_quote *quotes, char *str)
{
    BOOL        in_word;
    t_iterators it;

    it.i = 0;
    it.j = 0;
    in_word = 0;
    while (str[it.j])
    {
        if (!is_whitespace(str[it.j]) && in_word == FALSE)
        {
            in_word = TRUE;
            it.i++;
        }
        else if (!is_inside_quotes(quotes, it.j) && is_whitespace(str[it.j]))
        {
            in_word = FALSE;
        }
        it.j++;
    }
    return (it.i);
}

static char	*extract_word(t_db *db, t_quote *quotes, char *s, int *start)
{
    int			len;
    char		*word;
    int         i;

    i = *start;
    len = 0;
    while (s[i])
    {
        if (is_whitespace(s[i]) && !is_inside_quotes(quotes, i))
            break;
        len++;
        i++;
    }
    word = (char *)gc_malloc(db, (len + 1) * sizeof(char));
    if (word == NULL)
        return (NULL);
    ft_strlcpy(word, s + *start, len + 1);
    word[len] = '\0';
    *start = i;
    return (word);
}

char	**ft_new_split(t_db *db, t_quote *quotes, char *s)
{
	int		word_count;
	char	**result;
    t_iterators it;

	if (s == NULL)
		return (NULL);
	word_count = count_words(quotes, s);
	result = (char **)gc_malloc(db, (word_count + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	ft_bzero(&it, sizeof(t_iterators));
    while (it.i < word_count)
    {
        skip_open_spaces(quotes, s, &it.j);
        result[it.i] = extract_word(db, quotes, s, &it.j);
        if (result[it.i] == NULL)
            return (NULL);
        it.i++;
    }
    result[it.i] = NULL;
	return (result);
}
