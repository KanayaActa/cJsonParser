/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_value.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 07:12:17 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/26 11:13:09 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_VALUE_H
# define JSON_VALUE_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef enum e_json_type
{
	JSON_NULL,
	JSON_BOOL,
	JSON_NUMBER,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT
}								t_json_type;

struct	s_json_array;
struct	s_json_object;

typedef struct s_json_value
{
	t_json_type					type;
	union
	{
		bool					bool_value;
		double					number_value;
		char					*string_value;
		struct s_json_array		*array_value;
		struct s_json_object	*object_value;
	} u_;
}								t_json_value;

typedef struct s_json_array
{
	t_json_value				**items;
	int							size;
	int							capacity;
}								t_json_array;

typedef struct s_json_keyvalue
{
	char						*key;
	t_json_value				*value;
}								t_json_keyvalue;

typedef struct s_json_object
{
	t_json_keyvalue				*pairs;
	int							size;
	int							capacity;
}								t_json_object;

t_json_value					*json_value_new(t_json_type type);

void							json_value_free(t_json_value *val);
void							json_array_add(t_json_array *arr,
									t_json_value *item);
void							json_object_put(t_json_object *obj,
									const char *key, t_json_value *val);

#endif /* JSON_VALUE_H */
