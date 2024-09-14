/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:58:57 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 23:46:38 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

void	default_signals_behav(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	parent_behav(int signal)
{
	(void)signal;
}

void	handle_parent_signals(void)
{
	signal(SIGINT, parent_behav);
	signal(SIGQUIT, parent_behav);
}

void	heredoc_behave(int signal)
{
	t_db	*db;

	db = this();
	db->last_status = 130;
	if (signal == SIGINT)
	{
		ft_write(db, 2, "\n", 2);
		fd_void(db);
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
