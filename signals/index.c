/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 21:00:31 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/12 21:00:47 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

void	catch_feedback(t_db *db, int process_res)
{
	int	status;

	status = WEXITSTATUS(process_res);
	db->last_status = status;
	if (status && db->last_status == FAIL)
	{
		gc_void(db);
		ec_void(db);
		exit(FAIL);
	}
}

void	default_signals_behav(bool ignore_quit)
{
	signal(SIGINT, SIG_DFL);
	if (!ignore_quit)
		signal(SIGQUIT, SIG_DFL);
	else
		signal(SIGQUIT, SIG_IGN);
}

void	parent_behav(int signal)
{
	t_db *db;

	db = this();
	if (signal == SIGINT)
		ft_write(db, 2, "\n", 2);
	if (signal == SIGQUIT)
		ft_write(db, 2, "Quit\n", 5);
}

void	handle_parent_signals(void)
{
	signal(SIGINT, parent_behav);
	signal(SIGQUIT, parent_behav);
}

