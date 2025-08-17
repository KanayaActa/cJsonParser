/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_value.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 00:13:10 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/28 11:48:50 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_value.h"
#include "utils.h"

t_json_value	*json_value_new(t_json_type type)
{
	t_json_value	*v;

	v = (t_json_value *)ft_calloc(1, sizeof(t_json_value));
	if (!v)
		return (NULL);
	v->type = type;
	return (v);
}

void	json_array_add(t_json_array *arr, t_json_value *item)
{
	int				newcap;
	t_json_value	**newitems;

	if (!arr)
		return ;
	if (arr->size >= arr->capacity)
	{
		if (arr->capacity < 4)
			newcap = 4;
		else
			newcap = arr->capacity * 2;
		newitems = xrealloc(arr->items, sizeof(t_json_value *) * newcap);
		if (!newitems)
		{
			print_error("json_array_add: realloc failed\n");
			return ;
		}
		arr->items = newitems;
		arr->capacity = newcap;
	}
	arr->items[arr->size++] = item;
}

void	json_object_put(t_json_object *obj, const char *key, t_json_value *val)
{
	int				newcap;
	t_json_keyvalue	*newpairs;

	if (!obj)
		return ;
	if (obj->size >= obj->capacity)
	{
		if (obj->capacity < 4)
			newcap = 4;
		else
			newcap = obj->capacity * 2;
		newpairs = xrealloc(obj->pairs, sizeof(t_json_keyvalue) * newcap);
		if (!newpairs)
		{
			print_error("json_object_put: realloc failed\n");
			return ;
		}
		obj->pairs = newpairs;
		obj->capacity = newcap;
	}
	obj->pairs[obj->size].key = ft_strdup(key);
	obj->pairs[obj->size].value = val;
	obj->size++;
}
