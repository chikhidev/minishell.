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
    word = gc_malloc(db, (len + 1) * sizeof(char));
    CATCH_ONNULL(word, NULL);
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
    char   *tmp;

	CATCH_ONNULL(s, NULL);
	word_count = count_words(quotes, s);
	result = (char **)gc_malloc(db, (word_count + 1) * sizeof(char *));
    CATCH_ONNULL(result, NULL);
    ft_bzero(result, (word_count + 1) * sizeof(char *));
	ft_bzero(&it, sizeof(t_iterators));
    while (it.i < word_count)
    {
        skip_open_spaces(quotes, s, &it.j);
        tmp = extract_word(db, quotes, s, &it.j);
        CATCH_ONNULL(tmp, NULL);

        db->curr_type = validate_io(tmp, ft_strlen(tmp));
        if (db->curr_type != INVALID
            && it.i > 0)
        {
            skip_open_spaces(quotes, s, &it.j);

            tmp = extract_word(db, quotes, s, &it.j);

            if (db->curr_type == HEREDOC)
            {
                CATCH_ONFAILURE(open_heredoc(db, 
                    whithout_quotes(db, tmp)
                ), NULL);
            }
            else
            {
                CATCH_ONNULL(tmp, NULL);
                if (open_file(db, 
                    whithout_quotes(db, tmp)
                , db->curr_type, quotes) == FAILURE)
                    return (NULL);
            }
            word_count -= 2;
        }
        else
        {
            if (tmp[0] == '<' && tmp[1] == '<')
            {
                db->curr_type = HEREDOC;
                if (open_heredoc(db, whithout_quotes(db, tmp + 2)) == FAILURE)
                    return (NULL);
                word_count--;
            }
            else if (tmp[0] == '>' || tmp[0] == '<')
            {
                if (tmp[1] == '>' && tmp[0] == '>')
                    db->curr_type = APPENDFILE;
                else if (tmp[0] == '<')
                    db->curr_type = INPUTFILE;
                else
                    db->curr_type = OUTPUTFILE;

                if (open_file(db, 
                    whithout_quotes(db, tmp + 1 + 
                    (db->curr_type == APPENDFILE || db->curr_type == HEREDOC)
                    )
                , db->curr_type, quotes) == FAILURE)
                    return (NULL);
                word_count--;
            }
            else
            {
                result[it.i] = tmp;
                // CATCH_ONNULL(result[it.i], NULL);
                it.i++;
            }
        }
    }
	return (result);
}
