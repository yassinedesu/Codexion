/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 17:40:00 by yael-kha          #+#    #+#             */
/*   Updated: 2026/06/12 17:40:27 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

t_input *parsing(int argc, char **argv)
{
    t_input *data;

    data->number_of_coders = atoi(argv[1]);
    data->time_to_burnout = atoi(argv[2]);
    data->time_to_compile = atoi(argv[3]);
    data->time_to_debug = atoi(argv[4]);
    data->time_to_refactor = atoi(argv[5]);
    data->number_of_compiles_required = atoi(argv[6]);
    data->dongle_cooldown = atoi(argv[7]);
    data->scheduler = argv[8];
    return (data);
}