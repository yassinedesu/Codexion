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
	sleep_time(coder->sim->params->time_to_debug, coder->sim);
	return ;
}

int	coder_all(t_coder *coder)
{
	int	index;
	int	left;
	int	right;
	int	first;
	int	second;

	index = coder->coder_id - 1;
	left = index;
	right = (index + 1) % coder->sim->params->number_of_coders;
	first = left;
	second = right;
	if (index % 2 != 0)
	{
		first = right;
		second = left;
	}
	if (take_dongle(coder, first))
		return (1);
	if (take_dongle(coder, second))
	{
		drop_dongle(coder, first);
		return (1);
	}
	coder_compile(coder);
	drop_dongle(coder, left);
	drop_dongle(coder, right);
	coder_refactor(coder);
	coder_debug(coder);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	i = 0;
	while (i < coder->sim->params->number_of_compiles_required)
	{
		if (coder_all(coder) == 1)
			break ;
		i++;
	}
	return (NULL);
}
