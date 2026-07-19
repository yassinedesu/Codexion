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

void    *coder_routine(t_sim *sims, t_coder *coder)
{
    long    millisec;
    pthread_mutex_lock(&sims->log_mutex);
    millisec = timestamp_calc(sims->t_zero);
    print("%ld %d has taken a dongle", millisec, coder->coder_id);
    pthread_mutex_unlock(&sims->log_mutex);
    return ;
}