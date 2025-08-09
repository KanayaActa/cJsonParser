/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parser_stack.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 07:12:13 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/26 11:12:32 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_PARSER_STACK_H
# define JSON_PARSER_STACK_H

# include "json_lexer.h"
# include "json_value.h"

# define STACK_SIZE 256
# define BUFFER_SIZE 4096
# define ERROR 1

typedef enum e_parse_state
{
	STATE_VALUE,
	STATE_OBJECT_KEY_OR_END,
	STATE_OBJECT_COLON,
	STATE_OBJECT_VALUE,
	STATE_OBJECT_NEXT_OR_END,
	STATE_ARRAY_VALUE_OR_END,
	STATE_ARRAY_VALUE,
	STATE_ARRAY_NEXT_OR_END,
	STATE_EOF,
	STATE_END,
	STATE_INVALID
}						t_parse_state;

typedef struct s_json_parser_stack
{
	t_json_lexer		lexer;
	t_json_token		current;
	t_parse_state		state_stack[STACK_SIZE];
	int					state_top;
	t_json_value		*value_stack[STACK_SIZE];
	int					value_top;
	int					had_error;
}						t_json_parser_stack;

typedef void			(*t_parser_func)(t_json_parser_stack *ps);

typedef struct s_parser_transition
{
	t_parse_state		state;
	t_json_token_type	token;
	t_parser_func		func;
}						t_parser_transition;

void			json_parser_stack_init(t_json_parser_stack *ps, \
							const char *src);
void			json_parser_stack_destroy(t_json_parser_stack *ps);
t_json_value	*json_stack_parse(t_json_parser_stack *ps);

void			dump_state_stack(t_json_parser_stack *ps);
void			dump_value_stack(t_json_parser_stack *ps);
void			dump_current_token(t_json_parser_stack *ps);

void			push_state(t_json_parser_stack *ps, t_parse_state st);

void			push_state(t_json_parser_stack *ps, t_parse_state st);
t_parse_state	pop_state(t_json_parser_stack *ps);
t_parse_state	top_state(const t_json_parser_stack *ps);
void			push_value(t_json_parser_stack *ps, t_json_value *val);
t_json_value	*pop_value(t_json_parser_stack *ps);

void			json_advance(t_json_parser_stack *ps);
void			json_parser_stack_destroy(t_json_parser_stack *ps);

void			insert_object_keyvalue(t_json_parser_stack *ps);
void			insert_array_value(t_json_parser_stack *ps);

// state_value
void			parse_value_left_brace(t_json_parser_stack *ps);
void			parse_value_left_bracket(t_json_parser_stack *ps);
void			parse_value_string(t_json_parser_stack *ps);
void			parse_value_number(t_json_parser_stack *ps);
void			parse_value_true(t_json_parser_stack *ps);
void			parse_value_false(t_json_parser_stack *ps);
void			parse_value_null(t_json_parser_stack *ps);

// state_eof
void			parse_eof_end(t_json_parser_stack *ps);

// state_object_key_or_end
void			parse_obj_key_or_end_comma(t_json_parser_stack *ps);
void			parse_obj_key_or_end_right_brace(t_json_parser_stack *ps);
void			parse_obj_key_or_end_string(t_json_parser_stack *ps);

// state_object_colon
void			parse_obj_colon_colon(t_json_parser_stack *ps);

// state_object_value
void			parse_obj_value_left_brace(t_json_parser_stack *ps);
void			parse_obj_value_left_bracket(t_json_parser_stack *ps);
void			parse_obj_value_string(t_json_parser_stack *ps);
void			parse_obj_value_number(t_json_parser_stack *ps);
void			parse_obj_value_true(t_json_parser_stack *ps);
void			parse_obj_value_false(t_json_parser_stack *ps);
void			parse_obj_value_null(t_json_parser_stack *ps);

// state_object_next_or_end
void			parse_obj_next_or_end_any(t_json_parser_stack *ps);

// state_array_value_or_end
void			parse_array_value_or_end_right_bracket(t_json_parser_stack *ps);
void			parse_array_value_or_end_default(t_json_parser_stack *ps);

// state_array_value
void			parse_array_value_left_brace(t_json_parser_stack *ps);
void			parse_array_value_left_bracket(t_json_parser_stack *ps);
void			parse_array_value_string(t_json_parser_stack *ps);
void			parse_array_value_number(t_json_parser_stack *ps);
void			parse_array_value_true(t_json_parser_stack *ps);
void			parse_array_value_false(t_json_parser_stack *ps);
void			parse_array_value_null(t_json_parser_stack *ps);

// state_array_next_or_end
void			parse_array_next_or_end_any(t_json_parser_stack *ps);

// state_end
void			parse_state_end_any_token(t_json_parser_stack *ps);

// state_invalid
void			parse_state_invalid_any_token(t_json_parser_stack *ps);

#endif /* JSON_PARSER_STACK_H */
