/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 21:00:51 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/12 21:41:19 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"


void	heredoc_behave(int signal)
{
	t_db	*db;

	db = this();
	if (signal == SIGINT)
	{
		ft_write(db, 2, "\n", 2);
		gc_void(db);
		ec_void(db);
		exit(130);
	}
}

void	handle_here_doc_signals(void)
{
	signal(SIGINT, heredoc_behave);
	signal(SIGQUIT, SIG_IGN);
}


void	handle_sigint_prompt(int signum)
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
