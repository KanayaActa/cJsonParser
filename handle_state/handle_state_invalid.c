/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_state_invalid.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:57:26 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/24 23:58:40 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"

void	parse_state_invalid_any_token(t_json_parser_stack *ps)
{
	ps->had_error = 1;
}
