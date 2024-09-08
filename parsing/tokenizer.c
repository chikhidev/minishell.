#include "main.h"
#include "parsing.h"

void skip_open_spaces(t_quote *quotes, char *line, int *i)
{
	while (line[*i] && is_whitespace(line[*i]) && !is_inside_quotes_list(quotes, *i))
		(*i)++;
}

void add(t_db *db, char ***result, char *save)
{
	int size;

	// printf(UNDERLINE"saving->[%s]\n"RESET, save);

	if (!**result)
	{
		*result = (char **)gc_malloc(db, 2 * sizeof(char *));
		(*result)[0] = save;
		(*result)[1] = NULL;
		return;
	}
	size = 0;
	while ((*result)[size])
		size++;
	*result = (char **)gc_realloc(db, *result, (size + 2) * sizeof(char *));
	(*result)[size] = save;
	(*result)[size + 1] = NULL;
}

char **append_word(t_db *db, char **result, char *save)
{
	t_quote *q;
	char **splitted;
	int i;
	bool value_starts_with_dollar;

	if (ft_strlen(save) == 0)
	{
		return result;
	}

	q = NULL;
	if (track_quotes(db, &q, save) == FAILURE)
		return (NULL);

	// for (t_quote *Q_ = q; Q_; Q_ = Q_->next)
	// {
	// 	printf(BLUE"quote {ascii: %d, start: %d, end: %d}\n"RESET, Q_->ascii, Q_->start, Q_->end);
	// }

	value_starts_with_dollar = (ft_strsearch(save, '=') != NULL && (ft_strsearch(save, '=') + 1) != NULL && *(ft_strsearch(save, '=') + 1) == '$');
	expand(db, &save, &q);

	printf("expanded: [%s]\n", save);

	save = without_quotes(db, save, q);

	printf("without quotes: [%s]\n", save);

	if (!save)
	{
		add(db, &result, ft_strdup(db, ""));
		return (result);
	}

	if (db->split && !(result && result[0] && ft_strcmp(result[0], "export") == 0) && !value_starts_with_dollar)
	printf("split permission: %d\n", db->split);

	if (db->split && !(result && result[0] && ft_strcmp(result[0],
				"export") == 0) && !value_starts_with_dollar)
	{
		splitted = ft_split(db, save, " \t\n\r\v\f");
		i = 0;
		while (splitted && splitted[i])
		{
			add(db, &result, splitted[i]);
			i++;
		}
		return (result);
	}
	add(db, &result, save);
	return (result);
}

typedef struct s_tokenizer
{
	t_quote **quotes;
	char *line;
	char *save;
	char **result;
	bool read_write_perm;
	t_iterators it;
} t_tokenizer;

int saver(t_db *db, t_tokenizer *self)
{
	db->curr_type = validate_io(&self->line[self->it.i], 2);
	if (db->curr_type == INVALID)
		db->curr_type = validate_io(&self->line[self->it.i], 1);
	if (db->curr_type != INVALID)
	{
		self->it.i += 1 + (db->curr_type == HEREDOC || db->curr_type == APPENDFILE);
		skip_open_spaces(*self->quotes, self->line, &self->it.i);
		self->it.j = self->it.i;
		while (self->line[self->it.j] && !(is_whitespace(self->line[self->it.j]) && !is_inside_quotes_list(*self->quotes, self->it.j)) && !(validate_io(&self->line[self->it.j], 1) != INVALID || validate_io(&self->line[self->it.j], 2) != INVALID))
		{
			self->it.j++;
		}
		self->save = ft_substr(db, self->line, self->it.i, self->it.j - self->it.i);
		if (db->curr_type == HEREDOC && open_heredoc(db, self->save) == FAILURE)
		{
			db->exec_line = false;
			return (FAILURE);
		}
		else if (open_file(db, self->save, db->curr_type) == FAILURE)
			self->read_write_perm = false;
		self->save = NULL;
		self->it.i = self->it.j - 1;
	}
	self->result = append_word(db, self->result, self->save);
	if (!self->result)
		return FAILURE;
	self->save = NULL;
	return (SUCCESS);
}
bool match(const char *string, const char *pattern) {
    while (*pattern != '\0' && *string != '\0')
    {
        if (*pattern == '*')
        {
            // Skip consecutive '*'
            while (*pattern == '*')
                pattern++;
            // If '*' is the last character in the pattern, it's a match
            if (*pattern == '\0')
                return (true);
            // Try matching the rest of the pattern with every substring
            while (*string != '\0')
            {
                if (match(pattern, string))
                    return (true);
                string++;
            }
            return (false);
        }
        else if (*pattern != *string)
            return (false);
        pattern++;
        string++;
    }
    // Skip any remaining '*' in the pattern
    while (*pattern == '*')
        pattern++;
    // If we've reached the end of both strings, it's a match
    return (*pattern == '\0' && *string == '\0');
}
void handle_wildcard(t_db *db, char  ***result, char *pattern)
{
    DIR *curr_dir;
    struct dirent *entry;
    curr_dir = opendir(".");
    if (curr_dir)
    {
        entry = readdir(curr_dir);
        while (entry)
        {

            if (!starts_with(entry->d_name, ".") &&
                !starts_with(entry->d_name, "..") &&
                match(entry->d_name, pattern))
			{
                add(db, result, ft_strdup(db, entry->d_name));
			}
            entry = readdir(curr_dir);
        }
    }
}



char **tokenize(t_db *db, t_quote **quotes, char *s)
{
	t_tokenizer self;
	int len;
	bool is_open_whitespace_;
	bool is_open_io_;

	ft_bzero(&self, sizeof(t_tokenizer));
	self.quotes = quotes;
	self.line = s;
	self.result = (char **)gc_malloc(db, sizeof(char *));
	self.read_write_perm = true;
	skip_open_spaces(*self.quotes, self.line, &self.it.i);
	len = ft_strlen(self.line);

	while (self.it.i < len)
	{
		is_open_whitespace_ = is_open_whitespace(self.line, self.it.i,
												 *self.quotes);
		is_open_io_ = is_open_io(self.line, self.it.i, *self.quotes);

		if (!is_open_whitespace_ && !is_open_io_ && self.read_write_perm)
		{
			self.save = concat(db, self.save, self.line[self.it.i]);
		}
		else if (self.read_write_perm && saver(db, &self) == FAILURE)
			return (NULL);
		self.it.i++;
	}
	if (!inside_single_quote(*quotes, self.it.i) && contains(self.save, "*"))
	{
		handle_wildcard(db, &self.result, self.save);
		self.save = NULL;
	}
	else
		self.result = append_word(db, self.result, self.save);
	return (self.result);
}
