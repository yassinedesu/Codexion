/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 14:54:06 by yael-kha          #+#    #+#             */
/*   Updated: 2026/06/24 14:54:08 by yael-kha         ###   ########.fr       */
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
	t_input *data;

	data = parsed_args(argc, argv);
	if (!data)
	{
		printf("Invalid input!\n");
		return (1);
	}

	pthread_t thd;
	pthread_create(&thd, NULL, mon, NULL);
	pthread_join(thd, NULL);
	ft_free(data);
	return (0);
}