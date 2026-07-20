/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:10 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/17 15:02:12 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int	right_dongle;
	
	right_dongle = coder->coder_id % coder->sim->num_of_coders;
	if (coder->coder_id % 2 == 0)
		take_dongle(coder, coder->coder_id - 1);
	else
		take_dongle(coder, right_dongle);
	coder = (t_coder *)arg;
	print_status(coder, "has taken a dongle");
	return (NULL);
}
