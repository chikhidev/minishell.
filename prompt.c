/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 21:08:21 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/12 21:10:08 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

int	handle_prompt(t_db *db, char **line)
{
	struct sigaction	sa;
	char				*tmp;

	sa.sa_flags = 0;
	sa.sa_handler = handle_sigint_prompt;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	tmp = NULL;
	if (db->last_status == 0)
		tmp = ft_strjoin(db, "\001" GREEN "\002>_\001" RESET "\002", "$ ");
	else
		tmp = ft_strjoin(db, "\001" RED "\002>_\001" RESET "\002", "$ ");
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
	return (SUCCESS); /*nothing*/
}
