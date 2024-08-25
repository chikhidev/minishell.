#include "main.h"
#include "parsing.h"

/**
 * unused_arg - Check if an argument is unused
 * @db: The database
 * @arg: The argument to check
 * Return: true if the argument will be unsed later, false otherwise
*/
// bool will_be_unused_arg(t_db *db, char *arg)
// {
//     char *tmp;
//     char *final_form;
//     t_quote *quote;

//     tmp = ft_strdup(arg);
//     if (!tmp)
//         return false;

//     quote = NULL;
//     track_quotes(db, &quote, tmp);

//     final_form = ft_strdup(db, tmp);
//     expand(db, &final_form, &quote);

//     if (!quote && is_str_empty(db, final_form))
//     {
//         gc_free(db, final_form);
//         return true;
//     }

//     gc_free(db, final_form);
//     return false;
// }
