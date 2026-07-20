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

void	coder_all(t_coder *coder)
{
	int	left_dongle;
	int	right_dongle;

	left_dongle = coder->coder_id - 1;
	right_dongle = coder->coder_id % coder->sim->params->number_of_coders;
	coder_compile(coder);
	drop_dongle(coder, left_dongle);
	drop_dongle(coder, right_dongle);
	coder_refactor(coder);
	coder_debug(coder);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		i;
	int		right_dongle;

	coder = (t_coder *)arg;
	right_dongle = coder->coder_id % coder->sim->params->number_of_coders;
	i = 0;
	while (i < coder->sim->params->number_of_compiles_required)
	{
		if (coder->coder_id % 2 == 0)
		{
			take_dongle(coder, coder->coder_id - 1);
			take_dongle(coder, right_dongle);
		}
		else
		{
			take_dongle(coder, right_dongle);
			take_dongle(coder, coder->coder_id - 1);
		}
		coder_all(coder);
		i++;
	}
	return (NULL);
}
