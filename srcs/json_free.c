/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 00:07:11 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/25 00:23:56 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_value.h"

static void	free_json_string(t_json_value *val)
{
	free(val->u_.string_value);
}

static void	free_json_array(t_json_value *val)
{
	int	i;

	i = 0;
	if (val->u_.array_value)
	{
		while (i < val->u_.array_value->size)
		{
			json_value_free(val->u_.array_value->items[i]);
			i++;
		}
		free(val->u_.array_value->items);
		free(val->u_.array_value);
	}
}

static void	free_json_object(t_json_value *val)
{
	int	i;

	i = 0;
	if (val->u_.object_value)
	{
		while (i < val->u_.object_value->size)
		{
			free(val->u_.object_value->pairs[i].key);
			json_value_free(val->u_.object_value->pairs[i].value);
			i++;
		}
		free(val->u_.object_value->pairs);
		free(val->u_.object_value);
	}
}

void	json_value_free(t_json_value *val)
{
	if (!val)
		return ;
	if (val->type == JSON_STRING)
		free_json_string(val);
	else if (val->type == JSON_ARRAY)
		free_json_array(val);
	else if (val->type == JSON_OBJECT)
		free_json_object(val);
	free(val);
}
