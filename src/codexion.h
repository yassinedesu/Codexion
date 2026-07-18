/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 13:11:16 by yael-kha          #+#    #+#             */
/*   Updated: 2026/06/21 13:11:16 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_INT 2147483647

typedef struct s_input
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	char				*scheduler;
}						t_input;

typedef struct s_sim	t_sim;

typedef struct s_coder
{
	pthread_t			thread;
	int					coder_id;
	int					number_of_compiles;
	long				last_compile_start;
	t_sim				*sim;
}						t_coder;

typedef struct s_dongle
{
	pthread_mutex_t		mutex;
	int					id;
	bool				is_taken;
	long				last_time_used;
	pthread_cond_t		cond; // what's a pthread_cond_t cond?
}						t_dongle;

typedef struct s_sim
{
	int					num_of_coders;
	t_input				*params;
	t_coder				*coders;
	t_dongle			*dongles;
	struct timeval		t_zero;
	pthread_mutex_t		log_mutex;
	int					stop_flag;
	pthread_mutex_t		stop_mutex;
	// why this one & the stop_flag are related to each other
	// ,the stop_flag is the signal,
	// does that mean that stop_mutex is the one cleaning up ? 
	pthread_t			monitor;
}						t_sim;

// parser
t_input					*parsed_args(int argc, char **argv);

// monitor
void					*mon(void *arg);

#endif