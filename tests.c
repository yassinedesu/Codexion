/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 03:20:50 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/16 03:20:52 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 1. The Dongle Struct
typedef struct
{
	pthread_mutex_t	mutex;
	int				id;
}					t_dongle;

// 2. The Simulation Struct
typedef struct
{
	int				num_coders;
	t_dongle		*dongles;
}					t_sim;

// Struct to pass arguments to our thread
typedef struct
{
	int				coder_id;
	t_sim			*sim;
}					t_coder_args;

// 3. The Thread Routine
void	*coder_routine(void *arg)
{
	t_coder_args	*args;
	int				id;
	t_sim			*sim;
	int				left;
	int				right;

	args = (t_coder_args *)arg;
	id = args->coder_id;
	sim = args->sim;
	// Figure out the left and right indexes
	left = id;
	right = (id + 1) % sim->num_coders;
	// Force Coder 1 to start slightly later so Coder 0 grabs dongles first
	if (id == 1)
	{
		usleep(500000); // sleep for 0.5 seconds
	}
	printf("--> Coder %d: Trying to lock Left Dongle [%d]...\n", id + 1, left
		+ 1);
	pthread_mutex_lock(&sim->dongles[left].mutex);
	printf("--> Coder %d: LOCKED Left Dongle [%d]!\n", id + 1, left + 1);
	printf("--> Coder %d: Trying to lock Right Dongle [%d]...\n", id + 1, right
		+ 1);
	pthread_mutex_lock(&sim->dongles[right].mutex);
	printf("--> Coder %d: LOCKED Right Dongle [%d]! Working for 2 seconds...\n",
		id + 1, right + 1);
	// Simulate work
	sleep(2);
	printf("<-- Coder %d: Done! Unlocking Dongles [%d] and [%d].\n\n", id + 1,
		left + 1, right + 1);
	pthread_mutex_unlock(&sim->dongles[right].mutex);
	pthread_mutex_unlock(&sim->dongles[left].mutex);
	return (NULL);
}

int	main(void)
{
	int n = 10; // 2 Coders, 2 Dongles
	t_sim sim;
	sim.num_coders = n;
	sim.dongles = malloc(sizeof(t_dongle) * n);

	// Initialize the mutexes
	for (int i = 0; i < n; i++)
	{
		pthread_mutex_init(&sim.dongles[i].mutex, NULL);
		sim.dongles[i].id = i;
	}

	pthread_t threads[n];
	t_coder_args args[n];

	// Create the threads (Coders)
	for (int i = 0; i < n; i++)
	{
		args[i].coder_id = i;
		args[i].sim = &sim;
		pthread_create(&threads[i], NULL, coder_routine, &args[i]);
	}

	// Wait for them to finish
	for (int i = 0; i < n; i++)
	{
		pthread_join(threads[i], NULL);
	}

	// Cleanup
	for (int i = 0; i < n; i++)
	{
		pthread_mutex_destroy(&sim.dongles[i].mutex);
	}
	free(sim.dongles);

	return (0);
}