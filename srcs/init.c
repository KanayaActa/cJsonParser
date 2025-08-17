/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 07:52:24 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/28 11:35:38 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "json_parser_stack.h"
#include "json_value.h"
#include "utils.h"

void	json_lexer_init(t_json_lexer *lx, const char *src)
{
	lx->src = src;
	lx->pos = 0;
}

void	json_parser_stack_init(t_json_parser_stack *ps, const char *src)
{
	ft_memset(ps, 0, sizeof(*ps));
	json_lexer_init(&ps->lexer, src);
	ps->state_top = -1;
	ps->value_top = -1;
	ps->had_error = 0;
	ps->current = json_lexer_next(&ps->lexer);
	push_state(ps, STATE_VALUE);
}
