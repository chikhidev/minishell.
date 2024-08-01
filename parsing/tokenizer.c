# include "main.h"
# include "parsing.h"

void  skip_open_spaces(t_quote *quotes, char *line, int *i)
{
    while (line[*i] && is_whitespace(line[*i]) && !is_inside_quotes(quotes, *i))
        (*i)++;
}

char **append_word(t_db *db, char **result, char *save)
{
    int size;

    if (ft_strlen(save) == 0)
        return NULL;

    if (!*result)
    {
        result = (char **)gc_malloc(db, 2 * sizeof(char *));
        if (!result)
        {
            db->error = true;
            printf("Malloc failed\n");
            return (NULL);
        }
        result[0] = save;
        result[1] = NULL;
        return (result);
    }
    size = 0;
    while (result[size])
        size++;
    result = (char **)gc_realloc(db, result, (size + 2) * sizeof(char *));
    if (!result)
    {
        db->error = true;
        return (NULL);
    }
    result[size] = save;
    result[size + 1] = NULL;
    return (result);
}

char	**tokenize(t_db *db, t_quote **quotes, char *s)
{
	char	**result;
    t_iterators it;
    char    *save;
    int len;

    save = NULL;
	CATCH_ONNULL(s, NULL);
	result = (char **)gc_malloc(db, sizeof(char *));
    CATCH_ONNULL(result, NULL);
    result[0] = NULL;
    ft_bzero(&it, sizeof(t_iterators));
    skip_open_spaces(*quotes, s, &it.i);
    len = ft_strlen(s);
    while (it.i < len)
    {
        if (!(is_whitespace(s[it.i])
            && !is_inside_quotes(*quotes, it.i))
            && !(validate_io(&s[it.i], 1) != INVALID
            || validate_io(&s[it.i], 2) != INVALID))
        {
            save = concat(db, save, s[it.i]);
            CATCH_ONNULL(save, NULL);
        }
        else
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
                        && !is_inside_quotes(*quotes, it.j)
                    )
                    && !(validate_io(&s[it.j], 1) != INVALID
                    || validate_io(&s[it.j], 2) != INVALID))
                    it.j++;

                save = ft_substr(db, s, it.i, it.j - it.i);
                CATCH_ONNULL(save, NULL);

                if (db->curr_type == HEREDOC)
                {
                    CATCH_ONFAILURE(open_heredoc(db, 
                        save
                    ),NULL)
                }
                else
                {
                    open_file(db, save, db->curr_type, quotes);
                }
                gc_free(db, save);
                save = NULL;
                it.i = it.j - 1;
            }
            if (save)
            {
                track_quotes(db, quotes, save);
                CATCH_ONFAILURE(
                    expand(db, &save, quotes)
                , FAILURE);

                if (ft_strlen(save) > 0)
                {
                    result = append_word(db, result, save);
                    if (!result)
                    {
                        printf("failed to append word 2\n");
                        db->error = true;
                        return (NULL);
                    }
                }
                save = NULL;
            }
        }
        it.i++;
    }

    if (save)
    {
        track_quotes(db, quotes, save);
        CATCH_ONFAILURE(
            expand(db, &save, quotes)
        , FAILURE);

        

        if (ft_strlen(save) > 0)
        {
            result = append_word(db, result, save);
            if (!result)
            {
                printf("failed to append word 2\n");
                db->error = true;
                return (NULL);
            }
        }
    }

	return (result);
}

// ls<<DL<infile>outfile
