/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_lexer_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:45:39 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/24 22:31:58 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

t_json_token	lexer_null(t_json_lexer *lx)
{
	t_json_token	tok;

	(void)lx;
	tok.type = TOK_END;
	tok.text = NULL;
	return (tok);
}

t_json_token	lexer_left_bracket(t_json_lexer *lx)
{
	t_json_token	tok;

	tok.type = TOK_LEFT_BRACKET;
	tok.text = substrdup("[", 1);
	lx->pos++;
	return (tok);
}

t_json_token	lexer_right_bracket(t_json_lexer *lx)
{
	t_json_token	tok;

	tok.type = TOK_RIGHT_BRACKET;
	tok.text = substrdup("]", 1);
	lx->pos++;
	return (tok);
}

t_json_token	lexer_comma(t_json_lexer *lx)
{
	t_json_token	tok;

	tok.type = TOK_COMMA;
	tok.text = substrdup(",", 1);
	lx->pos++;
	return (tok);
}

t_json_token	lexer_colon(t_json_lexer *lx)
{
	t_json_token	tok;

	tok.type = TOK_COLON;
	tok.text = substrdup(":", 1);
	lx->pos++;
	return (tok);
}
