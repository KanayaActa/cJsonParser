/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state_object_next_or_end.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:39:59 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/24 23:42:03 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"

void	parse_obj_next_or_end_any(t_json_parser_stack *ps)
{
	insert_object_keyvalue(ps);
	if (ps->had_error)
		return ;
	if (ps->current.type == TOK_COMMA)
	{
		json_advance(ps);
		pop_state(ps);
		push_state(ps, STATE_OBJECT_KEY_OR_END);
	}
	else if (ps->current.type == TOK_RIGHT_BRACE)
	{
		json_advance(ps);
		pop_state(ps);
	}
	else
	{
		ps->had_error = 1;
	}
}
