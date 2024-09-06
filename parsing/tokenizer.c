# include "main.h"
# include "parsing.h"

void  skip_open_spaces(t_quote *quotes, char *line, int *i)
{
    while (line[*i] && is_whitespace(line[*i]) && !is_inside_quotes_list(quotes, *i))
        (*i)++;
}

void add(t_db *db, char ***result, char *tmp)
{
    int size;

    if (ft_strlen(tmp) == 0)
        return ;

    if (!**result)
    {
        *result = (char **)gc_malloc(db, 2 * sizeof(char *));
        (*result)[0] = tmp;
        (*result)[1] = NULL;
        return ;
    }
    size = 0;
    while ((*result)[size])
        size++;
    *result = (char **)gc_realloc(db, *result, (size + 2) * sizeof(char *));
    (*result)[size] = tmp;
    (*result)[size + 1] = NULL;
}

char **append_word(t_db *db, char **result, char *save, bool is_sub_call)
{
    char *tmp;
    t_quote *q;
    char **splitted;
    int i = 0;

    (void)is_sub_call;

    if (ft_strlen(save) == 0)
        return NULL;

    q = NULL;
    if (track_quotes(db, &q, save) == FAILURE)
        return NULL;

    if (expand(db, &save, &q) == FAILURE)
        return NULL;
    
    if (!q)
    {
        // split by spaces
        splitted = ft_split(db, save, " \t\n\r\v\f");
        i = 0;
        while (splitted && splitted[i])
        {
            add(db, &result, splitted[i]);
            i++;
        }
        return result;
    }
    
    tmp = without_quotes(db, save, q);

    add(db, &result, tmp);
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

	result = (char **)gc_malloc(db, sizeof(char *));
    result[0] = NULL;
    ft_bzero(&it, sizeof(t_iterators));
    skip_open_spaces(*quotes, s, &it.i);
    len = ft_strlen(s);
    while (it.i < len)
    {
        bool is_open_whitespace_ = is_open_whitespace(s, it.i, *quotes);
        bool is_open_io_ = is_open_io(s, it.i, *quotes);

        // if we have read_write_perm we can read and write
        if (!is_open_whitespace_ && !is_open_io_ && read_write_perm)
            save = concat(db, save, s[it.i]);
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
                        read_write_perm = false;
                }
                save = NULL;
                it.i = it.j - 1;
            }
            if (ft_strlen(save) > 0)
            {
                result = append_word(db, result, save, false);
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
        result = append_word(db, result, save, false);
        if (!result)
        {
            db->error = true;
            return (NULL);
        }
    }

	return (result);
}

// ls<<DL<infile>outfile
