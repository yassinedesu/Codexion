/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <your@mail.com>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 13:11:16 by yael-kha          #+#    #+#             */
/*   Updated: 2026/06/21 13:11:16 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define MAX_INT 2147483647

typedef struct s_input
{
	int			number_of_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			number_of_compiles_required;
	int			dongle_cooldown;
	char		*scheduler;
}				t_input;

typedef struct s_coder
{
	pthread_t	coder;
	int			coder_id;
	int			number_of_compiles;
	int			last_compile_start;
	t_input		*data;
}				t_coder;

// parser
t_input			*parsed_args(int argc, char **argv);

// monitor
void			*mon(void *arg);

#endif