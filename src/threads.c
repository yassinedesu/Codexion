/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 23:12:00 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/18 23:19:49 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_sim	*coder_create(t_sim *sims)
{
	int	i;

	i = 0;
	while (i < sims->params->number_of_coders)
	{
		if (pthread_create(&sims->coders[i].thread, NULL, coder_routine,
				sims->coders) != 0)
		{
			free_all(sims);
			return (NULL);
		}
		i++;
	}
	return (sims);
}
