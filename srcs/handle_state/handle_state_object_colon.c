/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state_object_colon.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:20:23 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/24 23:20:47 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"

void	parse_obj_colon_colon(t_json_parser_stack *ps)
{
	pop_state(ps);
	push_state(ps, STATE_OBJECT_VALUE);
	json_advance(ps);
}
