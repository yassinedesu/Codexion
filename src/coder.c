/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:10 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/17 15:02:12 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		left_dongle;
	int		right_dongle;

	coder = (t_coder *)arg;
	left_dongle = coder->coder_id - 1;
	right_dongle = coder->coder_id % coder->sim->params->number_of_coders;
	if (coder->coder_id % 2 == 0)
	{
		take_dongle(coder, left_dongle);
		take_dongle(coder, right_dongle);
	}
	else
	{
		take_dongle(coder, right_dongle);
		take_dongle(coder, left_dongle);
	}
	drop_dongle(coder, left_dongle);
	drop_dongle(coder, right_dongle);
	return (NULL);
}

void	coder_compile(t_coder *coder)
{
	print_status(coder, "is compiling");
	coder->last_compile_start = timestamp_calc(coder->sim->t_zero);
	sleep_time(coder->sim->params->time_to_compile, coder->sim);
	return ;
}

void	coder_refactor(t_coder *coder)
{
	print_status(coder, "is refactoring");
	sleep_time(coder->sim->params->time_to_refactor, coder->sim);
	return ;
}

void	coder_debug(t_coder *coder)
{
	print_status(coder, "is debugging");
	return ;
}
