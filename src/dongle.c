/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:26 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/22 03:37:49 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_stop(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->sim->stop_mutex);
	if (coder->sim->stop_flag == 1)
	{
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	return (0);
}

int	is_taken_condition(t_coder *coder, t_dongle *dongle)
{
	long	now;
	long	cd;
	long	prior;

	cd = coder->sim->params->dongle_cooldown;
	heap_push(dongle->wait_queue, coder->coder_id, get_priority(coder));
	while (1)
	{
		if (check_stop(coder, dongle))
			return (1);
		now = timestamp_calc(coder->sim->t_zero);
		if (dongle->is_taken == 0
			&& dongle->wait_queue->array[0].coder_id == coder->coder_id)
		{
			if (now >= dongle->last_time_used + cd)
				break ;
			pthread_mutex_unlock(&dongle->mutex);
			usleep(50);
			pthread_mutex_lock(&dongle->mutex);
			continue ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	heap_pop(dongle->wait_queue);
	return (0);
}

int	take_dongle(t_coder *coder, int dongle_id)
{
	t_dongle	*dongle;
	int			returned;

	dongle = &coder->sim->dongles[dongle_id];
	pthread_mutex_lock(&dongle->mutex);
	returned = is_taken_condition(coder, dongle);
	if (returned == 1)
		return (1);
	dongle->is_taken = 1;
	print_status(coder, "has taken a dongle");
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

void	drop_dongle(t_coder *coder, int dongle_id)
{
	long	timestamp_val;

	pthread_mutex_lock(&coder->sim->dongles[dongle_id].mutex);
	coder->sim->dongles[dongle_id].is_taken = 0;
	timestamp_val = timestamp_calc(coder->sim->t_zero);
	coder->sim->dongles[dongle_id].last_time_used = timestamp_val;
	pthread_cond_broadcast(&coder->sim->dongles[dongle_id].cond);
	pthread_mutex_unlock(&coder->sim->dongles[dongle_id].mutex);
	return ;
}

int	grab_dongles(t_coder *coder, int left, int right, int index)
{
	int	first;
	int	second;

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
	return (0);
}
