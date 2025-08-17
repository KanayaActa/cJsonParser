/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:29:46 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/28 11:31:22 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_lexer.h"
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char	*substrdup(const char *start, int length)
{
	char	*buf;

	buf = malloc(length + 1);
	if (!buf)
		return (NULL);
	ft_memcpy(buf, start, length);
	buf[length] = '\0';
	return (buf);
}

static t_json_token	try_lookup_table(char c, t_json_lexer *lx)
{
	t_json_token		tok;
	size_t				table_size;
	size_t				i;
	const t_lexer_table	parse_table[] = {
	{'\0', lexer_null},
	{'{', lexer_left_brace},
	{'}', lexer_right_brace},
	{'[', lexer_left_bracket},
	{']', lexer_right_bracket},
	{',', lexer_comma},
	{':', lexer_colon},
	{'"', lexer_string},
	};

	i = 0;
	table_size = sizeof(parse_table) / sizeof(parse_table[0]);
	while (i < table_size)
	{
		if (c == parse_table[i].ch)
			return (parse_table[i].func(lx));
		i++;
	}
	tok.type = TOK_UNKNOWN;
	tok.text = NULL;
	return (tok);
}

void	skip_whitespace(t_json_lexer *lx)
{
	while (ft_isspace((unsigned char)lx->src[lx->pos]))
		lx->pos++;
}

t_json_token	json_lexer_next(t_json_lexer *lx)
{
	t_json_token	tok;
	char			c;
	int				start;
	int				len;

	tok = (t_json_token){TOK_UNKNOWN, NULL};
	skip_whitespace(lx);
	c = lx->src[lx->pos];
	tok = try_lookup_table(c, lx);
	if (tok.type != TOK_UNKNOWN)
		return (tok);
	if ((c >= '0' && c <= '9') || c == '-' || c == '+')
		return (lexer_number(lx));
	else
	{
		start = lx->pos;
		while (ft_isalpha((unsigned char)lx->src[lx->pos]))
			lx->pos++;
		len = lx->pos - start;
		return (keyword_or_unknown(lx->src + start, len));
	}
}

void	json_token_destroy(t_json_token *tok)
{
	if (tok->text)
	{
		free(tok->text);
		tok->text = NULL;
	}
}
