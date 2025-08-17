/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 07:12:09 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/26 11:09:30 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json_parser_stack.h"
#include "utils.h"
#include <stdio.h>

int	main(void)
{
	const char	*json_str = "{\"name\": \"test\", \"value\": 42}";
	t_json_parser_stack	ps;
	t_json_value		*result;

	printf("Testing JSON Parser\n");
	printf("Input: %s\n", json_str);

	json_parser_stack_init(&ps, json_str);
	result = json_stack_parse(&ps);
	
	if (result && !ps.had_error)
	{
		printf("Parse successful!\n");
		json_value_free(result);
	}
	else
	{
		printf("Parse failed!\n");
	}

	json_parser_stack_destroy(&ps);
	return (0);
}