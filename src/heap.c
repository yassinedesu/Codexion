/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yael-kha <yael-kha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 15:02:40 by yael-kha          #+#    #+#             */
/*   Updated: 2026/07/22 17:02:56 by yael-kha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_higher(t_node a, t_node b)
{
	if (a.priority != b.priority)
		return (a.priority < b.priority);
	return (a.coder_id < b.coder_id);
}

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

t_heap	*heap_push(t_heap *heaps, long c_id, long prior)
{
	int		curr;
	int		parent;
	t_node	tmp;

	if (heaps->size >= heaps->capacity)
		return (heaps);
	heaps->array[heaps->size].coder_id = c_id;
	heaps->array[heaps->size].priority = prior;
	heaps->size++;
	curr = heaps->size - 1;
	while (curr > 0)
	{
		parent = (curr - 1) / 2;
		if (is_higher(heaps->array[curr], heaps->array[parent]))
		{
			tmp = heaps->array[curr];
			heaps->array[curr] = heaps->array[parent];
			heaps->array[parent] = tmp;
			curr = parent;
		}
		else
			break ;
	}
	return (heaps);
}

static void	poper_helper(t_heap *heaps, int current, int left, int right)
{
	int		small;
	t_node	node;

	while (1)
	{
		small = current;
		left = 2 * current + 1;
		right = 2 * current + 2;
		if (left < heaps->size && is_higher(heaps->array[left],
				heaps->array[small]))
			small = left;
		if (right < heaps->size && is_higher(heaps->array[right],
				heaps->array[small]))
			small = right;
		if (small != current)
		{
			node = heaps->array[small];
			heaps->array[small] = heaps->array[current];
			heaps->array[current] = node;
			current = small;
		}
		else
			break ;
	}
}

void	heap_pop(t_heap *heaps)
{
	int	current;
	int	left;
	int	right;

	current = 0;
	left = 0;
	right = 0;
	if (heaps->size == 0)
		return ;
	heaps->array[0] = heaps->array[heaps->size - 1];
	heaps->size--;
	poper_helper(heaps, current, left, right);
}
