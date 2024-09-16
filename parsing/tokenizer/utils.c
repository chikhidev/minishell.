/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 01:35:09 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/16 06:49:21 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

void	add(t_db *db, char ***result, char *save)
{
	size_t	size;

	if (!**result)
	{
		*result = (char **)gc_malloc(db, 2 * sizeof(char *));
		(*result)[0] = save;
		(*result)[1] = NULL;
		return ;
	}
	size = 0;
	while ((*result)[size])
		size++;
	*result = (char **)gc_realloc(db, *result, size * sizeof(char *), (size + 2)
			* sizeof(char *));
	(*result)[size] = save;
	(*result)[size + 1] = NULL;
}

bool	match(const char *string, const char *pattern)
{
	while (*pattern != '\0' && *string != '\0')
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			if (*pattern == '\0')
				return (true);
			while (*string != '\0')
			{
				if (match(string, pattern))
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
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0' && *string == '\0');
}

void	handle_wildcard(t_db *db, char ***result, char *pattern)
{
	DIR				*curr_dir;
	struct dirent	*entry;
	bool			matched;

	matched = false;
	curr_dir = opendir(".");
	if (curr_dir)
	{
		entry = readdir(curr_dir);
		while (entry)
		{
			if (!starts_with(entry->d_name, ".") && !starts_with(entry->d_name,
					"..") && match(entry->d_name, pattern))
			{
				matched = true;
				add(db, result, ft_strdup(db, entry->d_name));
			}
			entry = readdir(curr_dir);
		}
		closedir(curr_dir);
	}
	if (!matched)
		add(db, result, whithout_quotes_free_db(db, ft_strdup(db, pattern)));
}
