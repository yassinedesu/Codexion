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

/*Create a heap_push function that takes the heap pointer, the coder_id,
	and a long for priority.
  Insert the new node's data at the index of the current size,
	then increment the size. To sort it,
  write a while loop that compares this new node against its parent
  (found at index (current_index - 1)
	/ 2). If the new node has a smaller priority, swap them.
  Keep bubbling up until it hits the root or is no longer smaller than its parent.*/

t_heap	*init_heap(int cap)
{
	t_node	*node;
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
	heaps->capacity = cap;
	return (heaps);
}

t_heap	*heap_push(t_heap *heaps, int c_id, long prior)
{
	int	c_id;
	int	p_id;
	int	i;

	heaps->array[heaps->size].coder_id = c_id;
	heaps->array[heaps->size].priority = prior;
	heaps->size++;
	c_id = heaps->size - 1;
	while (c_id > 0)
	{
		p_id = (c_id - 1) / 2;
		if (heaps->array[c_id].priority < heaps->array[p_id].priority)
		{
			i = heaps->array[c_id].priority;
			heaps->array[c_id].priority = heaps->array[p_id].priority;
			heaps->array[p_id].priority = i;
			c_id = p_id;
		}
		else
			break ;
	}
	return (heaps);
}

void	poper_helper(t_heap *heaps, int current, int left, int right)
{

	int		small;
	t_node	*node;

    while (1)
	{
		small = current;
		left = 2 * current + 1;
		right = 2 * current + 2;
		if (left < heaps->size
			&& heaps->array[left].priority < heaps->array[small].priority)
			small = left;
		if (right < heaps->size
			&& heaps->array[right].priority < heaps->array[small].priority)
			small = right;
		if (small != current)
		{
			node = &heaps->array[small];
			heaps->array[small] = heaps->array[current];
			heaps->array[current] = *node;
		}
		if (small == current)
			break ;
	}
}

void	heap_pop(t_heap *heaps)
{
	int		current;
	int		left;
	int		right;
	t_node	*node;

	current = 0;
    left = 0;
    right = 0;
	if (heaps->size == 0)
		return ;
	heaps->array[0] = heaps->array[heaps->size - 1];
	heaps->size--;
	poper_helper(heaps, current, left, right);
}
