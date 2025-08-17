/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 07:12:13 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/26 11:12:32 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "json_parser_stack.h"
# include "json_value.h"

// デバッグフラグ
# ifdef DEBUG
#  define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#  define DEBUG_DUMP_STACK(ps) dump_parse_process(ps, "Current state")
# else
#  define DEBUG_PRINT(fmt, ...) do {} while(0)
#  define DEBUG_DUMP_STACK(ps) do {} while(0)
# endif

// table駆動方式のdump関数の型定義
typedef void (*dump_func_t)(const t_json_value *value, int indent);

// dump関数のテーブル構造体
typedef struct s_dump_table {
	t_json_type type;
	dump_func_t dump_func;
	const char *type_name;
} t_dump_table;

// デバッグ関数の宣言
void dump_parse_process(t_json_parser_stack *ps, const char *message);
void dump_json_value_table_driven(const t_json_value *value, int indent);
void dump_json_value(const t_json_value *value, int indent);

// 各型専用のdump関数
void dump_null_value(const t_json_value *value, int indent);
void dump_bool_value(const t_json_value *value, int indent);
void dump_number_value(const t_json_value *value, int indent);
void dump_string_value(const t_json_value *value, int indent);
void dump_array_value(const t_json_value *value, int indent);
void dump_object_value(const t_json_value *value, int indent);

#endif
