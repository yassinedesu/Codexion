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

int	main(int argc, char **argv)
{
	t_input	*param;
	t_sim	*arg_parse;
	int		i;

	param = parsed_args(argc, argv);
	if (param == NULL)
	{
		printf("Parsing has gone wrong!\n");
		return (-1);
	}
	arg_parse = init_sim(param);
	if (arg_parse == NULL)
	{
		printf("init_sim has failed!\n");
		return (-1);
	}
	arg_parse = init_mutexes(arg_parse);
	if (arg_parse == NULL)
	{
		printf("init_mutex has failed!\n");
		return (-1);
	}
	arg_parse = coder_create(arg_parse);
	if (arg_parse == NULL)
	{
		printf("coder_create has failed!\n");
		return (-1);
	}
	i = 0;
	while (i < arg_parse->params->number_of_coders)
	{
		pthread_join(arg_parse->coders[i].thread, NULL);
		i++;
	}
	pthread_join(arg_parse->monitor, NULL);
	mutex_cond_destroy(arg_parse, arg_parse->params->number_of_coders,
			arg_parse->params->number_of_coders);
	return (0);
}
