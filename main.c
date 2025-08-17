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
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// ファイルからJSON文字列を読み込む関数
static char *read_json_file(const char *filename)
{
	int fd;
	char *buffer;
	long file_size;
	ssize_t bytes_read;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return NULL;
	}

	// ファイルサイズを取得
	file_size = lseek(fd, 0, SEEK_END);
	if (file_size == -1)
	{
		perror("Error seeking file");
		close(fd);
		return NULL;
	}

	// ファイルポインタを先頭に戻す
	if (lseek(fd, 0, SEEK_SET) == -1)
	{
		perror("Error seeking file");
		close(fd);
		return NULL;
	}

	// バッファを確保
	buffer = malloc(file_size + 1);
	if (!buffer)
	{
		perror("Error allocating memory");
		close(fd);
		return NULL;
	}

	// ファイルを読み込む
	bytes_read = read(fd, buffer, file_size);
	if (bytes_read == -1)
	{
		perror("Error reading file");
		free(buffer);
		close(fd);
		return NULL;
	}

	buffer[bytes_read] = '\0';
	close(fd);
	return buffer;
}

int	main(void)
{
	const char	*filename = "test.json";
	char		*json_str;
	t_json_parser_stack	ps;
	t_json_value		*result;

	printf("Testing JSON Parser with file: %s\n", filename);

	// ファイルからJSONを読み込む
	json_str = read_json_file(filename);
	if (!json_str)
	{
		printf("Failed to read JSON file\n");
		return (1);
	}

	printf("Input JSON:\n%s\n", json_str);
	printf("Parsing...\n");

	json_parser_stack_init(&ps, json_str);
	result = json_stack_parse(&ps);
	
	if (result && !ps.had_error)
	{
		printf("\nParse successful!\n");
		printf("Parsed JSON structure:\n");
		dump_json_value_table_driven(result, 0);
		printf("\n");
		json_value_free(result);
	}
	else
	{
		printf("Parse failed!\n");
	}

	json_parser_stack_destroy(&ps);
	free(json_str);
	return (0);
}