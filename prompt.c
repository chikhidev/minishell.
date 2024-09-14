/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 07:02:06 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/14 21:37:22 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

void	handle_sigint(int signum)
{
	t_db	*db;

	db = this();
	(void)signum;
	ft_write(db, STDOUT_FILENO, "\n", 1);
	db->last_status = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	handle_prompt(t_db *db, char **line)
{
	char				*tmp;

	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	tmp = ft_strjoin(db, "\001" MAGENTA "\002>_\001" RESET "\002", "$ ");
	*line = readline(tmp);
	if (!*line)
	{
		put_fd(2, "exit\n");
		return (FAILURE);
	}
	tmp = ft_strdup(db, *line);
	free(*line);
	*line = tmp;
	if (ft_strlen(*line) > 0)
		add_history(*line);
	return (SUCCESS);
}
