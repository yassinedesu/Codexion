/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 03:20:50 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/16 03:20:52 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

void    *monitor(void *arg)
{
    (void)arg;
    printf("Barcaaaaaaaa\n");
    sleep(3);
    return (NULL);
}

int main(void)
{
    pthread_t   coder1;
    pthread_t   coder2;

    pthread_create(&coder1, NULL, monitor, NULL);
    pthread_join(coder1, NULL);
    
    pthread_create(&coder2, NULL, monitor, NULL);
    pthread_join(coder2, NULL);

    printf("Program was executed successfully!");
    
    return (0);
}