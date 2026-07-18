/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:45 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/18 20:11:34 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*init_dongles(t_sim *sims)
{
	int			i;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * sims->params->number_of_coders);
	if (!dongles)
		return (NULL);
	memset(dongles, 0, sizeof(t_dongle) * sims->params->number_of_coders);
	i = 0;
	while (i < sims->params->number_of_coders)
	{
		dongles[i].id = i;
		dongles[i].is_taken = false;
		dongles[i].last_time_used = 0;
		i++;
	}
	return (dongles);
}

t_coder	*init_coders(t_sim *sims)
{
	int		i;
	t_coder	*coders;

	coders = malloc(sizeof(t_coder) * sims->params->number_of_coders);
	if (!coders)
		return (NULL);
	memset(coders, 0, sizeof(t_coder) * sims->params->number_of_coders);
	i = 0;
	while (i < sims->params->number_of_coders)
	{
		coders[i].sim = sims;
		coders[i].coder_id = i + 1;
		coders[i].number_of_compiles = 0;
		coders[i].last_compile_start = 0;
		i++;
	}
	return (coders);
}

t_sim	*init_sim(t_input *param)
{
	t_sim	*inited_sim;

	inited_sim = malloc(sizeof(t_sim));
	if (!inited_sim)
		return (NULL);
	inited_sim->params = param;
	inited_sim->coders = init_coders(inited_sim);
	if (!inited_sim->coders)
		return (free_all(inited_sim), NULL);
	inited_sim->dongles = init_dongles(inited_sim);
	if (!inited_sim->dongles)
		return (free_all(inited_sim), NULL);
	gettimeofday(&inited_sim->t_zero, NULL);
	return (inited_sim);
}

void	*mutex_cond_destroy(t_sim *sims, int index1, int index2)
{
	int	i;

	i = 0;
	while (i < index1)
	{
		pthread_mutex_destroy(&sims->dongles[i].mutex);
		i++;
	}
	i = 0;
	while (i < index2)
	{
		pthread_cond_destroy(&sims->dongles[i].cond);
		i++;
	}
	pthread_mutex_destroy(&sims->log_mutex);
	pthread_mutex_destroy(&sims->stop_mutex);
	free_all(sims);
	return (NULL);
}

t_sim	*init_mutexes(t_sim *sims)
{
	int	i;

	if (pthread_mutex_init(&sims->log_mutex, NULL) != 0)
	{
		free_all(sims);
		return (NULL);
	}
	if (pthread_mutex_init(&sims->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sims->log_mutex);
		free_all(sims);
		return (NULL);
	}
	sims->stop_flag = 0;
	i = 0;
	while (i < sims->params->number_of_coders)
	{
		if (pthread_mutex_init(&sims->dongles[i].mutex, NULL) != 0)
			return (mutex_cond_destroy(sims, i, i - 1));
		if (pthread_cond_init(&sims->dongles[i].cond, NULL) != 0)
			return (mutex_cond_destroy(sims, i, i));
		i++;
	}
	return (sims);
}
