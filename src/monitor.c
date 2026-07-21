/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 17:40:00 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/11 04:14:02 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wake_all_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->params->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

int	trigger_burnout(t_sim *sim, int i)
{
	pthread_mutex_lock(&sim->stop_mutex);
	if (sim->stop_flag == 1)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		return (1);
	}
	sim->stop_flag = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	print_status(&sim->coders[i], "burned out");
	wake_all_coders(sim);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	int		last_comp;

	sim = (t_sim *)arg;
	while (1)
	{
		i = 0;
		while (i < sim->params->number_of_coders)
		{
			last_comp = sim->coders[i].last_compile_start;
			if (timestamp_calc(sim->t_zero)
				- last_comp >= sim->params->time_to_burnout)
			{
				if (trigger_burnout(sim, i))
					return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
