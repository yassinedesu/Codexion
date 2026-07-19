/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_corrected.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 23:41:21 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/18 23:41:22 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_sim	*init_sim(t_input *param)
{
	t_sim	*inited_sim;

	inited_sim = malloc(sizeof(t_sim));
	if (!inited_sim)
		return (NULL);
	// Bug 5 Fix: Zero out the struct so free_all doesn't free garbage
	memset(inited_sim, 0, sizeof(t_sim));
	inited_sim->params = param;
	inited_sim->coders = init_coders(inited_sim);
	if (!inited_sim->coders)
	{
		free_all(inited_sim);
		return (NULL);
	}
	inited_sim->dongles = init_dongles(inited_sim);
	if (!inited_sim->dongles)
	{
		free_all(inited_sim);
		return (NULL);
	}
	// Bug 3 Fix: Removed gettimeofday here. Moved to spawning phase.
	return (inited_sim);
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
		/* Bug 1 Fix: Pass 'i' instead of 'i
			- 1' so we don't orphan the last cond var*/
		if (pthread_mutex_init(&sims->dongles[i].mutex, NULL) != 0)
			return (mutex_cond_destroy(sims, i, i));
		if (pthread_cond_init(&sims->dongles[i].cond, NULL) != 0)
			return (mutex_cond_destroy(sims, i + 1, i));
		i++;
	}
	return (sims);
}

// Helper to cleanly back out if a pthread_create fails partway through
static void	handle_spawn_failure(t_sim *sim, int coders_spawned)
{
	int	i;

	// 1. Tell successfully spawned threads to stop immediately
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop_flag = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	// 2. Wait for them to exit
	i = 0;
	while (i < coders_spawned)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	// 3. Tear down all mutexes (we know they were all successfully initialized)
	// We pass the total number of coders to destroy everything.
	mutex_cond_destroy(sim, sim->params->number_of_coders,
		sim->params->number_of_coders);
}

int	spawn_threads(t_sim *sim)
{
	int	i;

	// Bug 3 Fix: Capture t_zero at the exact moment before threads spin up
	gettimeofday(&sim->t_zero, NULL);
	i = 0;
	while (i < sim->params->number_of_coders)
	{
		// Optional: Initialize coder's last compile start to t_zero here
		// sim->coders[i].last_compile_start = get_current_time_ms();
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine,
				&sim->coders[i]) != 0)
		{
			handle_spawn_failure(sim, i);
			return (0); // 0 indicates failure
		}
		i++;
	}
	// Spawn the monitor thread last, so it can immediately start watching
	// (Assuming you have a monitor_routine declared somewhere)
	/*
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
	{
		handle_spawn_failure(sim, sim->params->number_of_coders);
		return (0);
	}
	*/
	return (1); // 1 indicates success
}
