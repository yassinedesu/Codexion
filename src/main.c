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
	arg_parse = coder_create(arg_parse);
	if (arg_parse == NULL)
	{
		printf("coder_create has failed!\n");
		return (-1);
	}
	coder_routine(arg_parse->coders);
	return (0);
}
