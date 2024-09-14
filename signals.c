/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:58:57 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 21:47:33 by abchikhi         ###   ########.fr       */
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
	// t_db	*db;

	// db = this();
	(void)signal;
	// if (signal == SIGINT)
	// {
	// 	ft_write(db, 2, "\n", 2);
	// }
	// if (signal == SIGQUIT)
	// {
	// 	ft_write(db, 2, "Quit\n", 5);
	// }
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
