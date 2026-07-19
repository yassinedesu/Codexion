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

void    *coder_routine(void *arg)
{
    long    millisec;
    t_coder *coder = (t_coder *)arg;
    pthread_mutex_lock(&coder->sim->log_mutex);
    millisec = timestamp_calc(coder->sim->t_zero);
    printf("%ld %d has taken a dongle\n", millisec, coder->coder_id);
    pthread_mutex_unlock(&coder->sim->log_mutex);
    return (NULL);
}
