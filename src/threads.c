/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 23:12:00 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/18 23:19:49 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*spawn_fail(t_sim *sims, int running_threads)
{
	int	j;

	j = 0;
	pthread_mutex_lock(&sims->stop_mutex);
	sims->stop_flag = 1;
	pthread_mutex_unlock(&sims->stop_mutex);
	while (j < running_threads)
	{
		pthread_join(sims->coders[j].thread, NULL);
		j++;
	}
	mutex_cond_destroy(sims, sims->params->number_of_coders,
			sims->params->number_of_coders);
	return (NULL);
}

t_sim	*coder_create(t_sim *sims)
{
	int	i;

	i = 0;
	gettimeofday(&sims->t_zero, NULL);
	while (i < sims->params->number_of_coders)
	{
		if (pthread_create(&sims->coders[i].thread, NULL, coder_routine,
				&sims->coders[i]) != 0)
			return (spawn_fail(sims, i));
		i++;
	}
	if (pthread_create(&sims->monitor, NULL, monitor_routine, sims) != 0)
		return (spawn_fail(sims, sims->params->number_of_coders));
	return (sims);
}
