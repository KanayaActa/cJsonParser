/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state_array_value2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:51:29 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/25 00:25:18 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"

void	parse_array_value_true(t_json_parser_stack *ps)
{
	t_json_value	*bv;

	bv = json_value_new(JSON_BOOL);
	bv->u_.bool_value = true;
	push_value(ps, bv);
	pop_state(ps);
	push_state(ps, STATE_ARRAY_NEXT_OR_END);
	json_advance(ps);
}

void	parse_array_value_false(t_json_parser_stack *ps)
{
	t_json_value	*bv;

	bv = json_value_new(JSON_BOOL);
	bv->u_.bool_value = false;
	push_value(ps, bv);
	pop_state(ps);
	push_state(ps, STATE_ARRAY_NEXT_OR_END);
	json_advance(ps);
}

void	parse_array_value_null(t_json_parser_stack *ps)
{
	t_json_value	*nv;

	nv = json_value_new(JSON_NULL);
	push_value(ps, nv);
	pop_state(ps);
	push_state(ps, STATE_ARRAY_NEXT_OR_END);
	json_advance(ps);
}
