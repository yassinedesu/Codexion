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

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	int		last_comp;

	sim = (t_sim *)arg;
	last_comp = sim->params->time_to_burnout;
	while (1)
	{
		i = 0;
		while (i < sim->params->number_of_coders)
		{
			if (timestamp_calc(sim->t_zero) - last_comp 
				>= sim->params->time_to_burnout)
			{
				pthread_mutex_lock(&sim->stop_mutex);
				if (sim->stop_flag == 1)
				{
					pthread_mutex_unlock(&sim->stop_mutex);
					break;
				}
				sim->stop_flag = 1;
				print_status(&sim->coders[i], "burned out");
				// idk what helper function you're talking about
			}
			i++;
		}
		pthread_mutex_unlock(&sim->stop_mutex);
	}
	return (NULL);
}
