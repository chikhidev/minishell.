# include "main.h"
# include "parsing.h"

void  skip_open_spaces(t_quote *quotes, char *line, int *i)
{
    while (line[*i] && is_whitespace(line[*i]) && !is_inside_quotes_list(quotes, *i))
        (*i)++;
}

char **append_word(t_db *db, char **result, char *save)
{
    char *tmp;
    int size;
    t_quote *q;

    if (ft_strlen(save) == 0)
        return NULL;

    q = NULL;
    if (track_quotes(db, &q, save) == FAILURE)
    {
        return NULL;
    }

    if (expand(db, &save, &q) == FAILURE)
    {
        return NULL;
    }

    tmp = without_quotes(db, save, NULL);
    if (!*result)
    {
        result = (char **)gc_malloc(db, 2 * sizeof(char *));
        result[0] = tmp;
        result[1] = NULL;
        return (result);
    }
    size = 0;
    while (result[size])
        size++;
    result = (char **)gc_realloc(db, result, (size + 2) * sizeof(char *));
    result[size] = tmp;
    result[size + 1] = NULL;
    return (result);
}

char	**tokenize(t_db *db, t_quote **quotes, char *s)
{
	char	**result;
    t_iterators it;
    char    *save;
    int len;
    bool    read_write_perm;

    read_write_perm = true;
    save = NULL;
	CATCH_ONNULL(s, NULL);

	result = (char **)gc_malloc(db, sizeof(char *));

    result[0] = NULL;
    ft_bzero(&it, sizeof(t_iterators));
    skip_open_spaces(*quotes, s, &it.i);
    len = ft_strlen(s);
    while (it.i < len)
    {
        bool is_open_whitespace_ = is_open_whitespace(s, it.i, *quotes);
        bool is_open_io_ = is_open_io(s, it.i, *quotes);

        if (!is_open_whitespace_ && !is_open_io_ && read_write_perm // if we have read_write_perm we can read and write
        )
        {
            save = concat(db, save, s[it.i]);
        }
        else if (read_write_perm)
        {
            db->curr_type = validate_io(&s[it.i], 2);
            if (db->curr_type == INVALID)
                db->curr_type = validate_io(&s[it.i], 1);
            if (db->curr_type != INVALID)
            {
                it.i += 1 + (db->curr_type == HEREDOC || db->curr_type == APPENDFILE);
                skip_open_spaces(*quotes, s, &it.i);
                it.j = it.i;
                while (s[it.j] && !(
                        is_whitespace(s[it.j])
                        && !is_inside_quotes_list(*quotes, it.j)
                    )
                    && !(validate_io(&s[it.j], 1) != INVALID
                    || validate_io(&s[it.j], 2) != INVALID))
                    it.j++;

                save = ft_substr(db, s, it.i, it.j - it.i);
                CATCH_ONNULL(save, NULL);

                if (db->curr_type == HEREDOC)
                {
                    if (open_heredoc(db, save) == FAILURE)
                    {
                        db->exec_line = false;
                        return NULL;
                    }
                }
                else
                {
                    if (open_file(db, save, db->curr_type) == FAILURE)
                    {
                        read_write_perm = false;
                    }
                }
                gc_free(db, save);
                save = NULL;
                it.i = it.j - 1;
            }
            if (ft_strlen(save) > 0)
            {
                result = append_word(db, result, save);
                if (!result)
                {
                    db->error = true;
                    return (NULL);
                }
                
                save = NULL;
            }
        }
        it.i++;
    }

    if (ft_strlen(save) > 0)
    {
        result = append_word(db, result, save);
        if (!result)
        {
            db->error = true;
            return (NULL);
        }
    }

	return (result);
}

// ls<<DL<infile>outfile
