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
            db->error = TRUE;
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
        db->error = TRUE;
        return (NULL);
    }
    result[size] = save;
    result[size + 1] = NULL;

    return (result);
}

char	**tokenize(t_db *db, t_quote *quotes, char *s)
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
    skip_open_spaces(quotes, s, &it.i);
    len = ft_strlen(s);
    while (it.i < len)
    {
        if (!(is_whitespace(s[it.i])
            && !is_inside_quotes(quotes, it.i)))
        {
            save = concat(db, save, s[it.i]);
            CATCH_ONNULL(save, NULL);
        }
        else if (save)
        {
            result = append_word(db, result, save);
            if (!result)
            {
                printf("failed to append word 1\n");
                db->error = TRUE;
                return (NULL);
            }
            save = NULL;
        }
        it.i++;
    }

    if (save)
    {
        result = append_word(db, result, save);
        if (!result)
        {
            printf("failed to append word 2\n");
            db->error = TRUE;
            return (NULL);
        }
    }

	return (result);
}

// ls<<DL<infile>outfile


// char	**ft_new_split(t_db *db, t_quote *quotes, char *s)
// {
// 	int		word_count;
// 	char	**result;
//     t_iterators it;
//     char   *tmp;

// 	CATCH_ONNULL(s, NULL);
// 	word_count = count_words(quotes, s);
// 	result = (char **)gc_malloc(db, (word_count + 1) * sizeof(char *));
//     CATCH_ONNULL(result, NULL);
//     ft_bzero(result, (word_count + 1) * sizeof(char *));
// 	ft_bzero(&it, sizeof(t_iterators));
//     while (it.i < word_count)
//     {
//         skip_open_spaces(quotes, s, &it.j);
//         tmp = extract_word(db, quotes, s, &it.j);
//         CATCH_ONNULL(tmp, NULL);

//         db->curr_type = validate_io(tmp, ft_strlen(tmp));
//         if (db->curr_type != INVALID
//             && it.i > 0)
//         {
//             skip_open_spaces(quotes, s, &it.j);

//             tmp = extract_word(db, quotes, s, &it.j);
//             whithout_quotes(tmp);

//             if (db->curr_type == HEREDOC)
//             {
//                 CATCH_ONFAILURE(open_heredoc(db, 
//                     whithout_quotes(tmp)
//                 ), NULL);
//             }
//             else
//             {
//                 CATCH_ONNULL(tmp, NULL);
//                 if (open_file(db, 
//                     whithout_quotes(tmp)
//                 , db->curr_type, quotes) == FAILURE)
//                     return (NULL);
//             }
//             word_count -= 2;
//         }
//         else
//         {
//             if (tmp[0] == '<' && tmp[1] == '<')
//             {
//                 printf("heredoc\n");
//                 word_count--;
//             }
//             else if (tmp[0] == '>' || tmp[0] == '<')
//             {
//                 if (tmp[1] == '>' && tmp[0] == '>')
//                     db->curr_type = APPENDFILE;
//                 else if (tmp[0] == '<')
//                     db->curr_type = INPUTFILE;
//                 else
//                     db->curr_type = OUTPUTFILE;

//                 if (open_file(db, 
//                     whithout_quotes_free_db(db, tmp + 1 + 
//                     (db->curr_type == APPENDFILE)
//                     )
//                 , db->curr_type, quotes) == FAILURE)
//                     return (NULL);
//                 word_count--;
//             }
//             else
//             {
//                 result[it.i] = tmp;
//                 it.i++;
//             }
//         }
//     }
// 	return (result);
// }
