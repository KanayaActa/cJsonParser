/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 11:10:07 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/25 11:11:15 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static size_t	xrealloc_count_old(char **old)
{
	size_t	old_count;

	old_count = 0;
	while (old[old_count])
		old_count++;
	return (old_count);
}

void	*xrealloc(void *ptr, size_t size)
{
	char	**old;
	size_t	old_count;
	void	*newp;
	size_t	new_count;
	size_t	copy_count;

	if (!ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	old = (char **)ptr;
	old_count = xrealloc_count_old(old);
	newp = malloc(size);
	new_count = size / sizeof(char *);
	if (old_count + 1 < new_count)
		copy_count = old_count + 1;
	else
		copy_count = new_count;
	ft_memcpy(newp, old, copy_count * sizeof(char *));
	free(old);
	return (newp);
}
