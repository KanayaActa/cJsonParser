/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parser_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 22:55:15 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/24 22:59:14 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"
#include "debug.h"

void	json_advance(t_json_parser_stack *ps)
{
	json_token_destroy(&ps->current);
	ps->current = json_lexer_next(&ps->lexer);
}

void	json_parser_stack_destroy(t_json_parser_stack *ps)
{
	t_json_value	*v;

	json_token_destroy(&ps->current);
	while (ps->value_top >= 0)
	{
		v = pop_value(ps);
		json_value_free(v);
	}
}
