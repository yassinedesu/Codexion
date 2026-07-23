/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:01:59 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/22 17:02:49 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*free_partial_heaps(t_dongle *dongles, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(dongles[i].wait_queue->array);
		free(dongles[i].wait_queue);
		i++;
	}
	free(dongles);
	return (NULL);
}

void	free_queue(t_sim *sim)
{
	int	i;

	i = 0;
	if (!sim || !sim->dongles)
		return ;
	while (i < sim->params->number_of_coders)
	{
		if (sim->dongles[i].wait_queue)
		{
			free(sim->dongles[i].wait_queue->array);
			free(sim->dongles[i].wait_queue);
		}
		i++;
	}
}

void	free_all(t_sim *sim)
{
	if (!sim)
		return ;
	if (sim->coders)
	{
		free(sim->coders);
		sim->coders = NULL;
	}
	free_queue(sim);
	if (sim->dongles)
	{
		free(sim->dongles);
		sim->dongles = NULL;
	}
	if (sim->params)
	{
		free(sim->params);
		sim->params = NULL;
	}
	free(sim);
}
