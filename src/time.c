/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:03:11 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/17 15:03:12 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	timestamp_calc(struct timeval t_zero)
{
	long			t_zero_time;
	long			cur_time;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	cur_time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	t_zero_time = (t_zero.tv_sec * 1000) + (t_zero.tv_usec / 1000);
	return (cur_time - t_zero_time);
}
