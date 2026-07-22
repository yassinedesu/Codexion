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

static void	wake_all_coders(t_sim *sim)
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

static int	trigger_burnout(t_sim *sim, int i)
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

static int	check_coder(t_sim *sim, int i, int *finished)
{
	long	last_comp;
	int		is_done;
	int		num_compiles;

	is_done = 0;
	num_compiles = sim->params->number_of_compiles_required;
	pthread_mutex_lock(&sim->stop_mutex);
	last_comp = sim->coders[i].last_compile_start;
	if (sim->coders[i].number_of_compiles >= num_compiles)
	{
		(*finished)++;
		is_done = 1;
	}
	pthread_mutex_unlock(&sim->stop_mutex);
	if (is_done == 0 && timestamp_calc(sim->t_zero)
		- last_comp >= sim->params->time_to_burnout)
		return (trigger_burnout(sim, i));
	return (0);
}

static int	check_success(t_sim *sim, int finished)
{
	if (finished == sim->params->number_of_coders)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		sim->stop_flag = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		wake_all_coders(sim);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	int		finished;

	sim = (t_sim *)arg;
	while (1)
	{
		i = 0;
		finished = 0;
		while (i < sim->params->number_of_coders)
		{
			if (check_coder(sim, i, &finished))
				return (NULL);
			i++;
		}
		if (check_success(sim, finished))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
