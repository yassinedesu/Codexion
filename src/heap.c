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

/*Create a heap_push function that takes the heap pointer, the coder_id, and a long for priority.
  Insert the new node's data at the index of the current size, then increment the size. To sort it,
  write a while loop that compares this new node against its parent 
  (found at index (current_index - 1) / 2). If the new node has a smaller priority, swap them. 
  Keep bubbling up until it hits the root or is no longer smaller than its parent.*/

t_heap	*init_heap(int cap)
{
    t_node  *node;
	t_heap	*heaps;

	heaps = malloc(sizeof(t_heap));
	if (!heaps)
		return (NULL);
    node = malloc(sizeof(t_node) * cap);
    if (!node)
    {
        free(heaps);
        return (NULL);
    }
    heaps->array = node;
    heaps->size = 0;
	return (heaps);
}
t_heap  *heap_push(t_heap *heaps, int c_id, long prior)
{
    int i;
    int j;

    i = 0;
    heaps->array[heaps->size].coder_id = c_id;
    heaps->array[heaps->size].priority = prior;
    heaps->size++;
    while (i < heaps->size)
    {
        j = i + 1;
        while (j < heaps->size)
        {
        }
    }
}