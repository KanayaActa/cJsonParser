/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operate_stack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 22:41:28 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/24 22:43:55 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"

void	push_state(t_json_parser_stack *ps, t_parse_state st)
{
	ps->state_stack[++ps->state_top] = st;
}

t_parse_state	pop_state(t_json_parser_stack *ps)
{
	return (ps->state_stack[ps->state_top--]);
}

t_parse_state	top_state(const t_json_parser_stack *ps)
{
	return (ps->state_stack[ps->state_top]);
}

void	push_value(t_json_parser_stack *ps, t_json_value *val)
{
	ps->value_stack[++ps->value_top] = val;
}

t_json_value	*pop_value(t_json_parser_stack *ps)
{
	return (ps->value_stack[ps->value_top--]);
}
