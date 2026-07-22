/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:40 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/17 15:02:41 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// creating init_heap, heap_push & heap_pop

// Create an init_heap function that accepts an integer for capacity. It should allocate
// the t_heap structure, allocate the internal t_node array based on that capacity,
// set the size to 0, and return the allocated heap pointer.

t_heap	*init_heap(int cap)
{
    t_node  *node;
	t_heap	*heaps;

	heaps = malloc(sizeof(t_heap));
	if (!heaps)
		return (NULL);
    node = malloc(sizeof(t_node) * cap);
    if (!node)
        return (NULL);
    heap->array = node;
    heaps->size = 0;
	return (heaps);
}
