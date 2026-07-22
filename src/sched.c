/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sched.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:03:05 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/17 15:03:06 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_priority(t_coder *coder)
{
	long	last_compile;

	if (strcmp(coder->sim->params->scheduler, "fifo") == 0)
		return (timestamp_calc(coder->sim->t_zero));
	pthread_mutex_lock(&coder->sim->stop_mutex);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	return (last_compile + coder->sim->params->time_to_burnout);
}
