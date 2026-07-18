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

t_coder	*init_coders(t_sim *sims)
{
	int		i;
	t_coder	*coders;

	coders = malloc(sizeof(t_coder *) * sims->params->number_of_coders);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < sims->params->number_of_coders)
	{
		memset(&coders[i], NULL, sims->params->number_of_coders);
		i++;
	}
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
