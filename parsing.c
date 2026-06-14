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


#include <unistd.h>

static int ft_atoi(const char *str)
{
    int i;
    int res;

    i = 0;
    res = 0;
    while (str[i])
    {
        if (str[i] >= '0' && str[i] <= '9')
            res = (res * 10) + (str[i] - '0');
        else
            break;
        i++;
    }
    return res;
}