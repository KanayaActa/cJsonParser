/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state_eof.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:10:21 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/24 23:38:24 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"

void	parse_eof_end(t_json_parser_stack *ps)
{
	pop_state(ps);
	push_state(ps, STATE_END);
}
