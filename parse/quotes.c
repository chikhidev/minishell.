#include "../includes/main.h"

void    quotes_checker(t_root *root, int quotes_count, void (f)(char *, t_root *, int))
{
    for (int k = 0; k < quotes_count; k++)
    {
        if (root->quotes[k].end == -1 && k == quotes_count - 1)
        {
            root->tmp = readline(GREEN"quote> "RESET);
            if (!root->tmp)
                return error("readline failed", root);
            root->buffer = concat_str(root, root->buffer, '\n');
            f(root->tmp, root, quotes_count);
        }
    }
}

void    create_quoting(t_root *root, char _character_, int i, int *quotes_count)
{
    root->quotes = ft_realloc(root->quotes, sizeof(t_quote) * (*quotes_count + 1));
    if (!root->quotes)
        return error("malloc failed", root);
    root->quotes[*quotes_count].ascii = _character_;
    root->quotes[*quotes_count].start = i;
    root->quotes[*quotes_count].end = -1;
    (*quotes_count)++;
}

void    quotes_tracking(t_root *root, char _character_, int i, int *quotes_count)
{
    if ((_character_ == 34 || _character_ == 39)
        && root->quotes && *quotes_count > 0)
    {
        if (root->quotes[*quotes_count - 1].end == -1
            && root->quotes[*quotes_count - 1].ascii == _character_)
            root->quotes[*quotes_count - 1].end = i;
        else if (root->quotes[*quotes_count - 1].end != -1)
            create_quoting(root, _character_, i, quotes_count);
    }
    else if ((_character_ == 34 || _character_ == 39) && !root->quotes)
        create_quoting(root, _character_, i, quotes_count);
}

// "ls""'"
// "ls"'

// [0] = {ascii: 34, start: 0, end: -1}