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

void	take_dongle(t_coder *coder, int dongle_id)
{
	pthread_mutex_lock(&coder->sim->dongles[dongle_id].mutex);
	while (coder->sim->dongles[dongle_id].is_taken == 1)
		pthread_cond_wait(&coder->sim->dongles[dongle_id].cond,
			&coder->sim->dongles[dongle_id].mutex);
	coder->sim->dongles[dongle_id].is_taken = 1;
	print_status(coder, "has taken a dongle");
	pthread_mutex_unlock(&coder->sim->dongles[dongle_id].mutex);
	return ;
}

void	drop_dongle(t_coder *coder, int dongle_id)
{
    pthread_mutex_lock(&coder->sim->dongles[dongle_id].mutex);
    coder->sim->dongles[dongle_id].is_taken = 0;
    pthread_cond_signal(&coder->sim->dongles[dongle_id].cond);
    pthread_mutex_unlock(&coder->sim->dongles[dongle_id].mutex);
    return ;
}