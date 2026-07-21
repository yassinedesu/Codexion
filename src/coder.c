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

void    coder_compile(t_coder *coder)
{
    print_status(coder, "is compiling");
    pthread_mutex_lock(&coder->sim->stop_mutex);
    coder->last_compile_start = timestamp_calc(coder->sim->t_zero);
    coder->number_of_compiles++;
    pthread_mutex_unlock(&coder->sim->stop_mutex);
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

	index = coder->coder_id - 1;
	left = index;
	right = (index + 1) % coder->sim->params->number_of_coders;
	if (grab_dongles(coder, left, right, index))
		return (1);
	coder_compile(coder);
	drop_dongle(coder, left);
	drop_dongle(coder, right);
	coder_debug(coder);
	coder_refactor(coder);
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
