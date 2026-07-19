/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:40 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/17 15:02:41 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*spawn_fail(t_sim *sims, int running_threads)
{
    int i;
    int j;

    i = 0;

    while (i < running_threads)
	{
		
			pthread_mutex_lock(&sims->stop_mutex);
			sims->stop_flag = 1;
			pthread_mutex_unlock(&sims->stop_mutex);
			j = 0;
			while (j < i)
			{
				pthread_join(sims->coders[j].thread, NULL);
				j++;
			}
			mutex_cond_destroy(sims, sims->params->number_of_coders,
				sims->params->number_of_coders);
                i++;
    }
            return (NULL);
}