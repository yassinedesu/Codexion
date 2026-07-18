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

t_coder *init_coders(t_input *params)
{
    int     i;
    int     stat;
    t_coder *coders;

    coders = malloc(sizeof(t_coder *) * params->number_of_coders);
    if (!coders)
        return (NULL);
    i = 0;
    while (i < params->number_of_coders)
    {
        coders[i].coder_id = i + 1;
        coders[i].number_of_compiles = 0;
        coders[i].last_compile_start = 0;
        i++;
    }
    return (coders);
}
