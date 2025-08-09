/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state_array_value.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:48:23 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/25 13:12:58 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"
#include "parser.h"

void	parse_array_value_left_brace(t_json_parser_stack *ps)
{
	t_json_value	*obj;

	obj = json_value_new(JSON_OBJECT);
	obj->u_.object_value = ft_calloc(1, sizeof(t_json_object));
	push_value(ps, obj);
	pop_state(ps);
	push_state(ps, STATE_ARRAY_NEXT_OR_END);
	push_state(ps, STATE_OBJECT_KEY_OR_END);
	json_advance(ps);
}

void	parse_array_value_left_bracket(t_json_parser_stack *ps)
{
	t_json_value	*arr;

	arr = json_value_new(JSON_ARRAY);
	arr->u_.array_value = ft_calloc(1, sizeof(t_json_array));
	push_value(ps, arr);
	pop_state(ps);
	push_state(ps, STATE_ARRAY_NEXT_OR_END);
	push_state(ps, STATE_ARRAY_VALUE_OR_END);
	json_advance(ps);
}

void	parse_array_value_string(t_json_parser_stack *ps)
{
	t_json_value	*sv;

	sv = json_value_new(JSON_STRING);
	sv->u_.string_value = ft_strdup(ps->current.text);
	push_value(ps, sv);
	pop_state(ps);
	push_state(ps, STATE_ARRAY_NEXT_OR_END);
	json_advance(ps);
}

void	parse_array_value_number(t_json_parser_stack *ps)
{
	double			d;
	t_json_value	*nv;

	d = ft_strtod(ps->current.text, NULL);
	nv = json_value_new(JSON_NUMBER);
	nv->u_.number_value = d;
	push_value(ps, nv);
	pop_state(ps);
	push_state(ps, STATE_ARRAY_NEXT_OR_END);
	json_advance(ps);
}
