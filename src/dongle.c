/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:26 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/17 15:02:27 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongle(t_coder *coder, int dongle_id)
{
	t_dongle	*dongle;
	long		now;

	dongle = &coder->sim->dongles[dongle_id];
	pthread_mutex_lock(&dongle->mutex);
	while (1)
	{
		pthread_mutex_lock(&coder->sim->stop_mutex);
		if (coder->sim->stop_flag == 1)
		{
			pthread_mutex_unlock(&coder->sim->stop_mutex);
			pthread_mutex_unlock(&dongle->mutex);
			return (1);
		}
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		now = timestamp_calc(coder->sim->t_zero);
		if (dongle->is_taken == 0 && now >= dongle->last_time_used
			+ coder->sim->params->dongle_cooldown)
			break ;
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	dongle->is_taken = 1;
	print_status(coder, "has taken a dongle");
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
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

void	drop_dongle(t_coder *coder, int dongle_id)
{
	pthread_mutex_lock(&coder->sim->dongles[dongle_id].mutex);
	coder->sim->dongles[dongle_id].is_taken = 0;
	coder->sim->dongles[dongle_id].last_time_used
		= timestamp_calc(coder->sim->t_zero);
	pthread_cond_broadcast(&coder->sim->dongles[dongle_id].cond);
	pthread_mutex_unlock(&coder->sim->dongles[dongle_id].mutex);
	return ;
}
