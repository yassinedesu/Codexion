/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:45 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/17 15:02:46 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*init_dongles(t_sim *sims)
{
	int			i;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * sims->params->number_of_coders);
	if (!dongles)
		return (NULL);
	memset(dongles, 0, sizeof(t_dongle) * sims->params->number_of_coders);
	i = 0;
	while (i < sims->params->number_of_coders)
	{
		dongles[i].id = i;
		dongles[i].is_taken = false;
		dongles[i].last_time_used = 0;
		i++;
	}
	return (dongles);
}

t_coder	*init_coders(t_sim *sims)
{
	int		i;
	t_coder	*coders;

	coders = malloc(sizeof(t_coder) * sims->params->number_of_coders);
	if (!coders)
		return (NULL);
	memset(coders, 0, sizeof(t_coder) * sims->params->number_of_coders);
	i = 0;
	while (i < sims->params->number_of_coders)
	{
		coders[i].sim = sims;
		coders[i].coder_id = i + 1;
		coders[i].number_of_compiles = 0;
		coders[i].last_compile_start = 0;
		i++;
	}
	return (coders);
}
