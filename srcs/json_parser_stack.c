/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parser_stack.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 22:55:03 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/25 01:38:19 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* json_parser_stack.c */
#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void							handle_token(t_json_parser_stack *ps);

t_json_value	*json_stack_parse(t_json_parser_stack *ps)
{
	t_parse_state	st;

	while (!ps->had_error)
	{
		st = top_state(ps);
		if (st == STATE_END)
			break ;
		if (ps->current.type == TOK_END)
		{
			if (st == STATE_EOF)
			{
				pop_state(ps);
				push_state(ps, STATE_END);
				break ;
			}
			ps->had_error = 1;
			break ;
		}
		handle_token(ps);
	}
	if (ps->had_error || ps->value_top != 0)
		return (NULL);
	return (pop_value(ps));
}

static const t_parser_transition	g_parser_table[] = {
{STATE_VALUE, TOK_LEFT_BRACE, parse_value_left_brace},
{STATE_VALUE, TOK_LEFT_BRACKET, parse_value_left_bracket},
{STATE_VALUE, TOK_STRING, parse_value_string},
{STATE_VALUE, TOK_NUMBER, parse_value_number},
{STATE_VALUE, TOK_TRUE, parse_value_true},
{STATE_VALUE, TOK_FALSE, parse_value_false},
{STATE_VALUE, TOK_NULL, parse_value_null},
{STATE_EOF, TOK_END, parse_eof_end},
{STATE_OBJECT_KEY_OR_END, TOK_COMMA, parse_obj_key_or_end_comma},
{STATE_OBJECT_KEY_OR_END, TOK_RIGHT_BRACE,
	parse_obj_key_or_end_right_brace},
{STATE_OBJECT_KEY_OR_END, TOK_STRING, parse_obj_key_or_end_string},
{STATE_OBJECT_COLON, TOK_COLON, parse_obj_colon_colon},
{STATE_OBJECT_VALUE, TOK_LEFT_BRACE, parse_obj_value_left_brace},
{STATE_OBJECT_VALUE, TOK_LEFT_BRACKET, parse_obj_value_left_bracket},
{STATE_OBJECT_VALUE, TOK_STRING, parse_obj_value_string},
{STATE_OBJECT_VALUE, TOK_NUMBER, parse_obj_value_number},
{STATE_OBJECT_VALUE, TOK_TRUE, parse_obj_value_true},
{STATE_OBJECT_VALUE, TOK_FALSE, parse_obj_value_false},
{STATE_OBJECT_VALUE, TOK_NULL, parse_obj_value_null},
{STATE_OBJECT_NEXT_OR_END, TOK_LEFT_BRACE, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_RIGHT_BRACE, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_LEFT_BRACKET, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_STRING, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_NUMBER, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_TRUE, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_FALSE, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_NULL, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_COMMA, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_COLON, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_END, parse_obj_next_or_end_any},
{STATE_OBJECT_NEXT_OR_END, TOK_UNKNOWN, parse_obj_next_or_end_any},
{STATE_ARRAY_VALUE_OR_END, TOK_RIGHT_BRACKET,
	parse_array_value_or_end_right_bracket},
{STATE_ARRAY_VALUE_OR_END, TOK_LEFT_BRACE,
	parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_LEFT_BRACKET,
	parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_STRING, parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_NUMBER, parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_TRUE, parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_FALSE, parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_NULL, parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_COMMA, parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_COLON, parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_UNKNOWN, parse_array_value_or_end_default},
{STATE_ARRAY_VALUE_OR_END, TOK_END, parse_array_value_or_end_default},
{STATE_ARRAY_VALUE, TOK_LEFT_BRACE, parse_array_value_left_brace},
{STATE_ARRAY_VALUE, TOK_LEFT_BRACKET, parse_array_value_left_bracket},
{STATE_ARRAY_VALUE, TOK_STRING, parse_array_value_string},
{STATE_ARRAY_VALUE, TOK_NUMBER, parse_array_value_number},
{STATE_ARRAY_VALUE, TOK_TRUE, parse_array_value_true},
{STATE_ARRAY_VALUE, TOK_FALSE, parse_array_value_false},
{STATE_ARRAY_VALUE, TOK_NULL, parse_array_value_null},
{STATE_ARRAY_NEXT_OR_END, TOK_LEFT_BRACE, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_RIGHT_BRACE, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_LEFT_BRACKET, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_RIGHT_BRACKET, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_STRING, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_NUMBER, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_TRUE, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_FALSE, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_NULL, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_COMMA, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_COLON, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_END, parse_array_next_or_end_any},
{STATE_ARRAY_NEXT_OR_END, TOK_UNKNOWN, parse_array_next_or_end_any},
{STATE_END, TOK_LEFT_BRACE, parse_state_end_any_token},
{STATE_END, TOK_RIGHT_BRACE, parse_state_end_any_token},
{STATE_END, TOK_LEFT_BRACKET, parse_state_end_any_token},
{STATE_END, TOK_RIGHT_BRACKET, parse_state_end_any_token},
{STATE_END, TOK_COMMA, parse_state_end_any_token},
{STATE_END, TOK_COLON, parse_state_end_any_token},
{STATE_END, TOK_STRING, parse_state_end_any_token},
{STATE_END, TOK_NUMBER, parse_state_end_any_token},
{STATE_END, TOK_TRUE, parse_state_end_any_token},
{STATE_END, TOK_FALSE, parse_state_end_any_token},
{STATE_END, TOK_NULL, parse_state_end_any_token},
{STATE_END, TOK_UNKNOWN, parse_state_end_any_token},
{STATE_END, TOK_END, parse_state_end_any_token},
{STATE_INVALID, TOK_LEFT_BRACE, parse_state_invalid_any_token},
{STATE_INVALID, TOK_RIGHT_BRACE, parse_state_invalid_any_token},
{STATE_INVALID, TOK_LEFT_BRACKET, parse_state_invalid_any_token},
{STATE_INVALID, TOK_RIGHT_BRACKET, parse_state_invalid_any_token},
{STATE_INVALID, TOK_COMMA, parse_state_invalid_any_token},
{STATE_INVALID, TOK_COLON, parse_state_invalid_any_token},
{STATE_INVALID, TOK_STRING, parse_state_invalid_any_token},
{STATE_INVALID, TOK_NUMBER, parse_state_invalid_any_token},
{STATE_INVALID, TOK_TRUE, parse_state_invalid_any_token},
{STATE_INVALID, TOK_FALSE, parse_state_invalid_any_token},
{STATE_INVALID, TOK_NULL, parse_state_invalid_any_token},
{STATE_INVALID, TOK_UNKNOWN, parse_state_invalid_any_token},
{STATE_INVALID, TOK_END, parse_state_invalid_any_token},
};

static t_parser_func	find_parser_func(t_parse_state st, t_json_token_type tk)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = sizeof(g_parser_table) / sizeof(g_parser_table[0]);
	while (i < n)
	{
		if (g_parser_table[i].state == st && g_parser_table[i].token == tk)
			return (g_parser_table[i].func);
		i++;
	}
	return (NULL);
}

static void	handle_token(t_json_parser_stack *ps)
{
	t_parse_state		st;
	t_json_token_type	tktp;
	t_parser_func		func;

	st = top_state(ps);
	tktp = ps->current.type;
	func = find_parser_func(st, tktp);
	if (!func)
	{
		ps->had_error = 1;
		return ;
	}
	func(ps);
}
