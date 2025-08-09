/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_insert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 22:57:27 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/25 00:23:42 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"

void	insert_object_keyvalue(t_json_parser_stack *ps)
{
	t_json_value	*val;
	t_json_value	*keyv;
	t_json_value	*objv;

	val = pop_value(ps);
	keyv = pop_value(ps);
	objv = pop_value(ps);
	if (!objv || !keyv || !val)
	{
		ps->had_error = 1;
		return ;
	}
	if (objv->type != JSON_OBJECT || keyv->type != JSON_STRING)
	{
		ps->had_error = 1;
		return ;
	}
	json_object_put(objv->u_.object_value, keyv->u_.string_value, val);
	json_value_free(keyv);
	push_value(ps, objv);
}

void	insert_array_value(t_json_parser_stack *ps)
{
	t_json_value	*val;
	t_json_value	*arrv;

	val = pop_value(ps);
	arrv = pop_value(ps);
	if (!arrv || !val)
	{
		ps->had_error = 1;
		return ;
	}
	if (arrv->type != JSON_ARRAY)
	{
		ps->had_error = 1;
		return ;
	}
	json_array_add(arrv->u_.array_value, val);
	push_value(ps, arrv);
}
