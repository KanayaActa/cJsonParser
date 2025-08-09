/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state_array_value_or_end.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:43:42 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/24 23:44:26 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"

void	parse_array_value_or_end_right_bracket(t_json_parser_stack *ps)
{
	pop_state(ps);
	json_advance(ps);
}

void	parse_array_value_or_end_default(t_json_parser_stack *ps)
{
	pop_state(ps);
	push_state(ps, STATE_ARRAY_VALUE);
}
