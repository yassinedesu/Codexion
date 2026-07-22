/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:51 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/17 15:02:52 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, char *status)
{
	long	millisec;

	pthread_mutex_lock(&coder->sim->log_mutex);
	pthread_mutex_lock(&coder->sim->stop_mutex);
	if (coder->sim->stop_flag == 1 && strcmp(status, "burned out") != 0)
	{
		pthreat_mutex_unlock(&coder->sim-stop_mutex);
		pthreat_mutex_unlock(&coder->sim-log_mutex);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	millisec = timestamp_calc(coder->sim->t_zero);
	printf("%ld %d %s\n", millisec, coder->coder_id, status);
	pthread_mutex_unlock(&coder->sim->log_mutex);
	return ;
}
