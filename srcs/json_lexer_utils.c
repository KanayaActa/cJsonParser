/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_lexer_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:43:13 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/26 11:07:09 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

t_json_token	keyword_or_unknown(const char *str, int len)
{
	t_json_token	tok;

	tok.text = substrdup(str, len);
	if (len == 4 && ft_strncmp(str, "true", 4) == 0)
		tok.type = TOK_TRUE;
	else if (len == 5 && ft_strncmp(str, "false", 5) == 0)
		tok.type = TOK_FALSE;
	else if (len == 4 && ft_strncmp(str, "null", 4) == 0)
		tok.type = TOK_NULL;
	else
		tok.type = TOK_UNKNOWN;
	return (tok);
}

t_json_token	lexer_number(t_json_lexer *lx)
{
	t_json_token	tok;
	int				start;
	char			c;
	int				length;

	start = lx->pos;
	while (1)
	{
		c = lx->src[lx->pos];
		if (!c)
			break ;
		if ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.'
			|| c == 'e' || c == 'E')
			lx->pos++;
		else
			break ;
	}
	length = lx->pos - start;
	tok.type = TOK_NUMBER;
	tok.text = substrdup(lx->src + start, length);
	return (tok);
}

t_json_token	lexer_string(t_json_lexer *lx)
{
	t_json_token	tok;
	int				start;
	char			c;
	int				length;

	lx->pos++;
	start = lx->pos;
	while (1)
	{
		c = lx->src[lx->pos];
		if (!c)
			break ;
		if (c == '"')
		{
			length = lx->pos - start;
			tok.type = TOK_STRING;
			tok.text = substrdup(lx->src + start, length);
			lx->pos++;
			return (tok);
		}
		lx->pos++;
	}
	tok.type = TOK_UNKNOWN;
	tok.text = NULL;
	return (tok);
}

t_json_token	lexer_left_brace(t_json_lexer *lx)
{
	t_json_token	tok;

	tok.type = TOK_LEFT_BRACE;
	tok.text = substrdup("{", 1);
	lx->pos++;
	return (tok);
}

t_json_token	lexer_right_brace(t_json_lexer *lx)
{
	t_json_token	tok;

	tok.type = TOK_RIGHT_BRACE;
	tok.text = substrdup("}", 1);
	lx->pos++;
	return (tok);
}
