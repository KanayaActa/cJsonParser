/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 07:12:13 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/26 11:12:32 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include <stdio.h>

// インデント出力用のヘルパー関数
static void print_indent(int indent)
{
	for (int i = 0; i < indent; i++)
		printf("  ");
}

// 各型専用のdump関数の実装
void dump_null_value(const t_json_value *value, int indent)
{
	(void)value;
	print_indent(indent);
	printf("null");
}

void dump_bool_value(const t_json_value *value, int indent)
{
	print_indent(indent);
	printf("%s", value->u_.bool_value ? "true" : "false");
}

void dump_number_value(const t_json_value *value, int indent)
{
	print_indent(indent);
	printf("%g", value->u_.number_value);
}

void dump_string_value(const t_json_value *value, int indent)
{
	print_indent(indent);
	printf("\"%s\"", value->u_.string_value);
}

void dump_array_value(const t_json_value *value, int indent)
{
	t_json_array *arr = value->u_.array_value;
	
	print_indent(indent);
	printf("[\n");
	for (int i = 0; i < arr->size; i++)
	{
		dump_json_value_table_driven(arr->items[i], indent + 1);
		if (i < arr->size - 1)
			printf(",");
		printf("\n");
	}
	print_indent(indent);
	printf("]");
}

void dump_object_value(const t_json_value *value, int indent)
{
	t_json_object *obj = value->u_.object_value;
	
	print_indent(indent);
	printf("{\n");
	for (int i = 0; i < obj->size; i++)
	{
		print_indent(indent + 1);
		printf("\"%s\": ", obj->pairs[i].key);
		dump_json_value_table_driven(obj->pairs[i].value, indent + 1);
		if (i < obj->size - 1)
			printf(",");
		printf("\n");
	}
	print_indent(indent);
	printf("}");
}

// dump関数のテーブル定義
static const t_dump_table dump_table[] = {
	{JSON_NULL, dump_null_value, "null"},
	{JSON_BOOL, dump_bool_value, "boolean"},
	{JSON_NUMBER, dump_number_value, "number"},
	{JSON_STRING, dump_string_value, "string"},
	{JSON_ARRAY, dump_array_value, "array"},
	{JSON_OBJECT, dump_object_value, "object"}
};

// table駆動方式のdump関数
void dump_json_value_table_driven(const t_json_value *value, int indent)
{
	if (!value)
	{
		printf("(null value)");
		return;
	}
	
	// テーブルから対応するdump関数を検索
	for (size_t i = 0; i < sizeof(dump_table) / sizeof(dump_table[0]); i++)
	{
		if (dump_table[i].type == value->type)
		{
			dump_table[i].dump_func(value, indent);
			return;
		}
	}
	
	// 対応する型が見つからない場合
	print_indent(indent);
	printf("(unknown type: %d)", value->type);
}

// 値フォーマット関数の型定義
typedef void (*value_formatter_t)(const t_json_value *val, const char *type_name);

// 値フォーマット関数の実装
static void format_bool_value(const t_json_value *val, const char *type_name) {
	printf("%s (%s)", type_name, val->u_.bool_value ? "true" : "false");
}

static void format_number_value(const t_json_value *val, const char *type_name) {
	printf("%s (%g)", type_name, val->u_.number_value);
}

static void format_string_value(const t_json_value *val, const char *type_name) {
	printf("%s (\"%s\")", type_name, val->u_.string_value);
}

static void format_simple_value(const t_json_value *val, const char *type_name) {
	(void)val; // 未使用パラメータを抑制
	printf("%s", type_name);
}

// 従来のswitch-case方式のdump関数（後方互換性のため）
void dump_json_value(const t_json_value *value, int indent)
{
	dump_json_value_table_driven(value, indent);
}

// STATE名とトークン名のテーブル構造体
typedef struct s_name_table {
	t_parse_state value;
	const char *name;
} t_state_name_table;

typedef struct s_token_name_table {
	t_json_token_type value;
	const char *name;
} t_token_name_table;

typedef struct s_json_type_name_table {
	t_json_type value;
	const char *name;
} t_json_type_name_table;

// STATE名を文字列に変換する関数（table駆動方式）
static const char *get_state_name(t_parse_state state)
{
	static const t_state_name_table state_table[] = {
		{STATE_VALUE, "STATE_VALUE"},
		{STATE_OBJECT_KEY_OR_END, "STATE_OBJECT_KEY_OR_END"},
		{STATE_OBJECT_COLON, "STATE_OBJECT_COLON"},
		{STATE_OBJECT_VALUE, "STATE_OBJECT_VALUE"},
		{STATE_OBJECT_NEXT_OR_END, "STATE_OBJECT_NEXT_OR_END"},
		{STATE_ARRAY_VALUE_OR_END, "STATE_ARRAY_VALUE_OR_END"},
		{STATE_ARRAY_VALUE, "STATE_ARRAY_VALUE"},
		{STATE_ARRAY_NEXT_OR_END, "STATE_ARRAY_NEXT_OR_END"},
		{STATE_EOF, "STATE_EOF"},
		{STATE_END, "STATE_END"},
		{STATE_INVALID, "STATE_INVALID"}
	};
	
	for (size_t i = 0; i < sizeof(state_table) / sizeof(state_table[0]); i++)
	{
		if (state_table[i].value == state)
			return state_table[i].name;
	}
	return "UNKNOWN_STATE";
}

// トークン名を文字列に変換する関数（table駆動方式）
static const char *get_token_name(t_json_token_type token)
{
	static const t_token_name_table token_table[] = {
		{TOK_END, "TOK_END"},
		{TOK_LEFT_BRACE, "TOK_LEFT_BRACE"},
		{TOK_RIGHT_BRACE, "TOK_RIGHT_BRACE"},
		{TOK_LEFT_BRACKET, "TOK_LEFT_BRACKET"},
		{TOK_RIGHT_BRACKET, "TOK_RIGHT_BRACKET"},
		{TOK_COMMA, "TOK_COMMA"},
		{TOK_COLON, "TOK_COLON"},
		{TOK_STRING, "TOK_STRING"},
		{TOK_NUMBER, "TOK_NUMBER"},
		{TOK_TRUE, "TOK_TRUE"},
		{TOK_FALSE, "TOK_FALSE"},
		{TOK_NULL, "TOK_NULL"},
		{TOK_UNKNOWN, "TOK_UNKNOWN"}
	};
	
	for (size_t i = 0; i < sizeof(token_table) / sizeof(token_table[0]); i++)
	{
		if (token_table[i].value == token)
			return token_table[i].name;
	}
	return "UNKNOWN_TOKEN";
}

// パース過程のデバッグ出力
void dump_parse_process(t_json_parser_stack *ps, const char *message)
{
	printf("\n=== %s ===\n", message);
	
	// 現在のトークンを表示
	printf("Current Token: ");
	if (ps->current.type == TOK_STRING)
		printf("%s(\"%s\")", get_token_name(ps->current.type), ps->current.text);
	else if (ps->current.type == TOK_NUMBER)
		printf("%s(%s)", get_token_name(ps->current.type), ps->current.text);
	else
		printf("%s", get_token_name(ps->current.type));
	printf("\n");
	
	// 状態スタックの内容を上から下へ出力
	printf("State Stack (top -> bottom):\n");
	for (int i = ps->state_top; i >= 0; i--)
	{
		printf("  [%d]: %s\n", i, get_state_name(ps->state_stack[i]));
	}
	
			// 値スタックの内容を上から下へ出力
	printf("Value Stack (top -> bottom):\n");
	for (int i = ps->value_top; i >= 0; i--)
	{
		t_json_value *val = ps->value_stack[i];
		printf("  [%d]: ", i);
		if (val)
		{
			// JSON型名を文字列に変換するテーブル
			static const t_json_type_name_table json_type_table[] = {
				{JSON_NULL, "JSON_NULL"},
				{JSON_BOOL, "JSON_BOOL"},
				{JSON_NUMBER, "JSON_NUMBER"},
				{JSON_STRING, "JSON_STRING"},
				{JSON_ARRAY, "JSON_ARRAY"},
				{JSON_OBJECT, "JSON_OBJECT"}
			};
			
			// テーブルから型名を検索
			const char *type_name = "UNKNOWN_TYPE";
			for (size_t j = 0; j < sizeof(json_type_table) / sizeof(json_type_table[0]); j++)
			{
				if (json_type_table[j].value == val->type)
				{
					type_name = json_type_table[j].name;
					break;
				}
			}
			
			// 値フォーマット関数のテーブル
			static const struct {
				t_json_type type;
				value_formatter_t formatter;
			} value_formatter_table[] = {
				{JSON_BOOL, format_bool_value},
				{JSON_NUMBER, format_number_value},
				{JSON_STRING, format_string_value},
				{JSON_NULL, format_simple_value},
				{JSON_ARRAY, format_simple_value},
				{JSON_OBJECT, format_simple_value}
			};
			
			// テーブルからフォーマッターを検索
			value_formatter_t formatter = format_simple_value;
			for (size_t k = 0; k < sizeof(value_formatter_table) / sizeof(value_formatter_table[0]); k++)
			{
				if (value_formatter_table[k].type == val->type)
				{
					formatter = value_formatter_table[k].formatter;
					break;
				}
			}
			
			formatter(val, type_name);
		}
		else
		{
			printf("null");
		}
		printf("\n");
	}
	printf("================\n");
}
