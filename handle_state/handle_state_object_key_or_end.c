/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state_object_key_or_end.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:14:00 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/28 11:35:52 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"
#include "parser.h"

void	parse_obj_key_or_end_comma(t_json_parser_stack *ps)
{
	json_advance(ps);
	if (ps->current.type == TOK_RIGHT_BRACE)
	{
		ps->had_error = 1;
		return ;
	}
}

void	parse_obj_key_or_end_right_brace(t_json_parser_stack *ps)
{
	json_advance(ps);
	pop_state(ps);
}

void	parse_obj_key_or_end_string(t_json_parser_stack *ps)
{
	t_json_value	*keyv;

	keyv = json_value_new(JSON_STRING);
	keyv->u_.string_value = ft_strdup(ps->current.text);
	push_value(ps, keyv);
	pop_state(ps);
	push_state(ps, STATE_OBJECT_COLON);
	json_advance(ps);
}
