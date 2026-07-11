/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 14:54:06 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/11 04:14:56 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_free(t_input *data)
{
	free(data->scheduler);
	free(data);
}

int	main(int argc, char **argv)
{
	t_input		*data;
	pthread_t	thd;

	data = parsed_args(argc, argv);
	if (!data)
	{
		printf("Invalid input!\n");
		return (1);
	}
	pthread_create(&thd, NULL, mon, NULL);
	pthread_join(thd, NULL);
	ft_free(data);
	return (0);
}
