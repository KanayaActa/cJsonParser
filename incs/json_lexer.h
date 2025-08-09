/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_lexer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 07:12:09 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/26 11:09:30 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_LEXER_H
# define JSON_LEXER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef enum e_json_token_type
{
	TOK_END,
	TOK_LEFT_BRACE,
	TOK_RIGHT_BRACE,
	TOK_LEFT_BRACKET,
	TOK_RIGHT_BRACKET,
	TOK_COMMA,
	TOK_COLON,
	TOK_STRING,
	TOK_NUMBER,
	TOK_TRUE,
	TOK_FALSE,
	TOK_NULL,
	TOK_UNKNOWN
}						t_json_token_type;

typedef struct s_json_token
{
	t_json_token_type	type;
	char				*text;
}						t_json_token;

typedef struct s_json_lexer
{
	const char			*src;
	int					pos;
}						t_json_lexer;

typedef t_json_token	(*t_lex_func)(t_json_lexer *lx);

typedef struct s_lexer_table
{
	char				ch;
	t_lex_func			func;
}						t_lexer_table;

void					json_lexer_init(t_json_lexer *lx, const char *src);
t_json_token			json_lexer_next(t_json_lexer *lx);
void					json_token_destroy(t_json_token *tok);

t_json_token			keyword_or_unknown(const char *str, int len);
t_json_token			lexer_null(t_json_lexer *lx);
t_json_token			lexer_number(t_json_lexer *lx);
t_json_token			lexer_string(t_json_lexer *lx);
t_json_token			lexer_left_brace(t_json_lexer *lx);
t_json_token			lexer_right_brace(t_json_lexer *lx);
t_json_token			lexer_left_bracket(t_json_lexer *lx);
t_json_token			lexer_right_bracket(t_json_lexer *lx);
t_json_token			lexer_comma(t_json_lexer *lx);
t_json_token			lexer_colon(t_json_lexer *lx);
char					*substrdup(const char *start, int length);

#endif