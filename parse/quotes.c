#include "../includes/main.h"

void    quotes_checker(t_root *root, int quotes_count, void (f)(char *, t_root *, int))
{
    printf("quotes_count: %d\n", quotes_count);
    for (int k = 0; k < quotes_count; k++)
    {
        if (root->quotes[k].end == -1 && k == quotes_count - 1)
        {
            root->tmp = readline(YELLOW"quote> "RESET);
            if (!root->tmp) // ------------------------- should be !root->tmp just temporary to test leaks
                error("readline failed", root);
            root->buffer = concat_str(root, root->buffer, '\n');
            f(root->tmp, root, quotes_count);
        }
    }
}

void    quotes_tracking(t_root *root, char _character_, int i, int *quotes_count)
{
    if (_character_ == '\'' || _character_ == '\"')
    {
        root->putted = 0;
        for (int k = 0; k < *quotes_count; k++)
        {
            if (root->quotes[k].ascii == _character_ && root->quotes[k].end == -1)
            {
                root->quotes[k].end = i;
                root->putted = 1;
            }
        }
        if (!root->putted)
        {
            root->quotes = ft_realloc(root->quotes, sizeof(t_quote) * (*quotes_count + 1));
            if (!root->quotes)
                error("malloc failed", root);
            root->quotes[*quotes_count].ascii = _character_;
            root->quotes[*quotes_count].start = i;
            root->quotes[*quotes_count].end = -1;
            (*quotes_count)++;
        }
    }
}
