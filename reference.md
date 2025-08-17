├── a.out
├── debug.c
├── debug.h
├── json
    ├── array.json
    ├── error1.json
    ├── error2.json
    ├── error3.json
    ├── error4.json
    ├── error5.json
    ├── ex.json
    ├── float.json
    ├── miniRT.json
    ├── multi.json
    ├── multi_obj.json
    ├── obj.json
    ├── only_brackets.json
    ├── reserved.json
    └── simple.json
├── json_lexer.c
├── json_lexer.h
├── json_parser_stack.c
├── json_parser_stack.h
├── json_value.c
├── json_value.h
├── main.c
└── out


/a.out:
--------------------------------------------------------------------------------
https://raw.githubusercontent.com/KanayaActa/c_json_paser/3eb97b277f80de8cd618a49e9f995bccbbc94703/a.out


--------------------------------------------------------------------------------
/debug.c:
--------------------------------------------------------------------------------
 1 | #include <stdio.h>
 2 | #include "json_parser_stack.h"
 3 | #include "json_value.h"
 4 | 
 5 | /* 
 6 |  * t_parse_state は以下のような値（例）をとると仮定します。
 7 |  * 必要に応じて実際の定義に合わせてください。
 8 |  *
 9 |  * typedef enum {
10 |  *     STATE_VALUE,
11 |  *     STATE_OBJECT_KEY_OR_END,
12 |  *     STATE_OBJECT_COLON,
13 |  *     STATE_OBJECT_VALUE,
14 |  *     STATE_OBJECT_NEXT_OR_END,
15 |  *     STATE_ARRAY_VALUE_OR_END,
16 |  *     STATE_ARRAY_VALUE,
17 |  *     STATE_ARRAY_NEXT_OR_END,
18 |  *     STATE_EOF,
19 |  *     STATE_END,
20 |  *     STATE_INVALID
21 |  * } t_parse_state;
22 |  */
23 | 
24 | /* 状態を文字列に変換するヘルパー */
25 | static const char* state_to_string(t_parse_state st) {
26 |     switch(st) {
27 |         case STATE_VALUE:              return "STATE_VALUE";
28 |         case STATE_OBJECT_KEY_OR_END:  return "STATE_OBJECT_KEY_OR_END";
29 |         case STATE_OBJECT_COLON:       return "STATE_OBJECT_COLON";
30 |         case STATE_OBJECT_VALUE:       return "STATE_OBJECT_VALUE";
31 |         case STATE_OBJECT_NEXT_OR_END: return "STATE_OBJECT_NEXT_OR_END";
32 |         case STATE_ARRAY_VALUE_OR_END: return "STATE_ARRAY_VALUE_OR_END";
33 |         case STATE_ARRAY_VALUE:        return "STATE_ARRAY_VALUE";
34 |         case STATE_ARRAY_NEXT_OR_END:  return "STATE_ARRAY_NEXT_OR_END";
35 |         case STATE_EOF:                return "STATE_EOF";
36 |         case STATE_END:                return "STATE_END";
37 |         case STATE_INVALID:            return "STATE_INVALID";
38 |         default:                       return "UNKNOWN_STATE";
39 |     }
40 | }
41 | 
42 | /* JSONの型を文字列に変換するヘルパー */
43 | static const char* type_to_string(t_json_type type) {
44 |     switch(type) {
45 |         case JSON_NULL:    return "JSON_NULL";
46 |         case JSON_BOOL:    return "JSON_BOOL";
47 |         case JSON_NUMBER:  return "JSON_NUMBER";
48 |         case JSON_STRING:  return "JSON_STRING";
49 |         case JSON_ARRAY:   return "JSON_ARRAY";
50 |         case JSON_OBJECT:  return "JSON_OBJECT";
51 |         default:           return "UNKNOWN_TYPE";
52 |     }
53 | }
54 | 
55 | /* 状態スタックの内容を上から下へ出力 */
56 | void dump_state_stack(t_json_parser_stack *ps) {
57 |     printf("State Stack (top -> bottom):\n");
58 |     for (int i = ps->state_top; i >= 0; i--) {
59 |         printf("  [%d]: %s\n", i, state_to_string(ps->state_stack[i]));
60 |     }
61 | }
62 | 
63 | /* 値スタックの内容を上から下へ出力 */
64 | void dump_value_stack(t_json_parser_stack *ps) {
65 |     printf("Value Stack (top -> bottom):\n");
66 |     for (int i = ps->value_top; i >= 0; i--) {
67 |         t_json_value *val = ps->value_stack[i];
68 |         printf("  [%d]: %s", i, type_to_string(val->type));
69 |         if (val->type == JSON_STRING && val->u.string_value) {
70 |             printf(" (\"%s\")", val->u.string_value);
71 |         } else if (val->type == JSON_NUMBER) {
72 |             printf(" (%g)", val->u.number_value);
73 |         }
74 |         printf("\n");
75 |     }
76 | }
77 | 
78 | /* デバッグ用に、現在のトークンも出力する関数（オプション） */
79 | void dump_current_token(t_json_parser_stack *ps) {
80 |     const char *tn = "???";
81 |     switch(ps->current.type) {
82 |         case TOK_END:           tn = "END"; break;
83 |         case TOK_LEFT_BRACE:    tn = "{"; break;
84 |         case TOK_RIGHT_BRACE:   tn = "}"; break;
85 |         case TOK_LEFT_BRACKET:  tn = "["; break;
86 |         case TOK_RIGHT_BRACKET: tn = "]"; break;
87 |         case TOK_COMMA:         tn = ","; break;
88 |         case TOK_COLON:         tn = ":"; break;
89 |         case TOK_STRING:        tn = "STRING"; break;
90 |         case TOK_NUMBER:        tn = "NUMBER"; break;
91 |         case TOK_TRUE:          tn = "TRUE"; break;
92 |         case TOK_FALSE:         tn = "FALSE"; break;
93 |         case TOK_NULL:          tn = "NULL"; break;
94 |         case TOK_UNKNOWN:       tn = "UNKNOWN"; break;
95 |     }
96 |     printf("Current Token: %s [%s]\n", tn, ps->current.text ? ps->current.text : "(null)");
97 | }
98 | 


--------------------------------------------------------------------------------
/debug.h:
--------------------------------------------------------------------------------
 1 | #ifndef JSON_PARSER_STACK_DEBUG_H
 2 | #define JSON_PARSER_STACK_DEBUG_H
 3 | 
 4 | #include "json_parser_stack.h"
 5 | #include "json_value.h"
 6 | 
 7 | /*
 8 |  * 以下の関数は、パーサ内部の状態をデバッグ出力するためのものです。
 9 |  *
10 |  * dump_state_stack() は、状態スタックの中身を上から下へ出力します。
11 |  * dump_value_stack() は、値スタックの中身（各 JSON 値の型および必要に応じた情報）を上から下へ出力します。
12 |  * dump_current_token() は、現在のトークンの種別とテキストを出力します。
13 |  */
14 | 
15 | void dump_state_stack(t_json_parser_stack *ps);
16 | void dump_value_stack(t_json_parser_stack *ps);
17 | void dump_current_token(t_json_parser_stack *ps);
18 | 
19 | #endif /* JSON_PARSER_STACK_DEBUG_H */
20 | 


--------------------------------------------------------------------------------
/json/array.json:
--------------------------------------------------------------------------------
1 | {
2 | 	"key":[2,3,4]
3 | }


--------------------------------------------------------------------------------
/json/error1.json:
--------------------------------------------------------------------------------
1 | {
2 | 	l
3 | }


--------------------------------------------------------------------------------
/json/error2.json:
--------------------------------------------------------------------------------
1 | {
2 | 	"s":true,
3 | }


--------------------------------------------------------------------------------
/json/error3.json:
--------------------------------------------------------------------------------
1 | {
2 | 	,
3 | }


--------------------------------------------------------------------------------
/json/error4.json:
--------------------------------------------------------------------------------
1 | {
2 | 	"key":[1,]
3 | }


--------------------------------------------------------------------------------
/json/error5.json:
--------------------------------------------------------------------------------
1 | {
2 | 	"obj":{
3 | 		"obj":"a",
4 | 	}
5 | }


--------------------------------------------------------------------------------
/json/ex.json:
--------------------------------------------------------------------------------
 1 | {
 2 | 	"key":1,
 3 | 	"key2":2,
 4 | 	"obj1": {
 5 | 		"key4":[2,3],
 6 | 		"key5":false,
 7 | 		"obj3": {
 8 | 			"key7": null
 9 | 		}
10 | 	},
11 | 	"obj2": {
12 | 		"key9": "NULL"
13 | 	}
14 | }
15 | 


--------------------------------------------------------------------------------
/json/float.json:
--------------------------------------------------------------------------------
1 | {
2 | 	"key":0.8
3 | }


--------------------------------------------------------------------------------
/json/miniRT.json:
--------------------------------------------------------------------------------
 1 | {
 2 | 	"ambient": {
 3 | 	  "brightness": 0.2,
 4 | 	  "color": [255, 255, 255]
 5 | 	},
 6 | 	"camera": {
 7 | 	  "position": [-50.0, 0, 20],
 8 | 	  "direction": [0, 0, 1],
 9 | 	  "fov": 70
10 | 	},
11 | 	"lights": [
12 | 	  {
13 | 		"position": [-40.0, 50.0, 0.0],
14 | 		"brightness": 0.6,
15 | 		"color": [10, 0, 255]
16 | 	  }
17 | 	],
18 | 	"objects": {
19 | 	  "spheres": [
20 | 		{
21 | 		  "center": [0.0, 0.0, 20.6],
22 | 		  "diameter": 12.6,
23 | 		  "color": [10, 0, 255]
24 | 		}
25 | 	  ],
26 | 	  "planes": [
27 | 		{
28 | 		  "point": [0.0, 0.0, -10.0],
29 | 		  "normal": [0.0, 1.0, 0.0],
30 | 		  "color": [0, 0, 225]
31 | 		}
32 | 	  ],
33 | 	  "cylinders": [
34 | 		{
35 | 		  "center": [50.0, 0.0, 20.6],
36 | 		  "axis": [0.0, 0.0, 1.0],
37 | 		  "diameter": 14.2,
38 | 		  "height": 21.42,
39 | 		  "color": [10, 0, 255]
40 | 		}
41 | 	  ]
42 | 	}
43 | }
44 | 


--------------------------------------------------------------------------------
/json/multi.json:
--------------------------------------------------------------------------------
1 | {
2 | 	"key":12,
3 | 	"key2":24,
4 | 	"key3":36
5 | }


--------------------------------------------------------------------------------
/json/multi_obj.json:
--------------------------------------------------------------------------------
 1 | {
 2 | 	"obj1": {
 3 | 		"key1":"value1",
 4 | 		"key2":"value2"
 5 | 	},
 6 | 	"obj2": {
 7 | 		"key1":"value1",
 8 | 		"key2":"value2"
 9 | 	}
10 | }


--------------------------------------------------------------------------------
/json/obj.json:
--------------------------------------------------------------------------------
1 | {
2 | 	"obj":{
3 | 		"key":"value"
4 | 	}
5 | }


--------------------------------------------------------------------------------
/json/only_brackets.json:
--------------------------------------------------------------------------------
1 | {
2 | }


--------------------------------------------------------------------------------
/json/reserved.json:
--------------------------------------------------------------------------------
1 | {
2 | 	"key1":true,
3 | 	"key2":null,
4 | 	"key3":false
5 | }


--------------------------------------------------------------------------------
/json/simple.json:
--------------------------------------------------------------------------------
1 | {
2 | 	"key":12
3 | }


--------------------------------------------------------------------------------
/json_lexer.c:
--------------------------------------------------------------------------------
  1 | #include "json_lexer.h"
  2 | #include <ctype.h>
  3 | #include <stdlib.h>
  4 | #include <string.h>
  5 | 
  6 | // 部分文字列コピー（終端付き）
  7 | static char *substrdup(const char *start, int length) {
  8 |     char *buf = malloc(length + 1);
  9 |     if (!buf) return NULL;
 10 |     memcpy(buf, start, length);
 11 |     buf[length] = '\0';
 12 |     return buf;
 13 | }
 14 | 
 15 | void json_lexer_init(t_json_lexer *lx, const char *src) {
 16 |     lx->src = src;
 17 |     lx->pos = 0;
 18 | }
 19 | 
 20 | static void skip_whitespace(t_json_lexer *lx) {
 21 |     while (isspace((unsigned char)lx->src[lx->pos])) {
 22 |         lx->pos++;
 23 |     }
 24 | }
 25 | 
 26 | static t_json_token keyword_or_unknown(const char *str, int len) {
 27 |     t_json_token tok;
 28 |     tok.text = substrdup(str, len);
 29 |     if (len == 4 && strncmp(str, "true", 4) == 0) {
 30 |         tok.type = TOK_TRUE;
 31 |     } else if (len == 5 && strncmp(str, "false", 5) == 0) {
 32 |         tok.type = TOK_FALSE;
 33 |     } else if (len == 4 && strncmp(str, "null", 4) == 0) {
 34 |         tok.type = TOK_NULL;
 35 |     } else {
 36 |         tok.type = TOK_UNKNOWN;
 37 |     }
 38 |     return tok;
 39 | }
 40 | 
 41 | static t_json_token lex_number(t_json_lexer *lx) {
 42 |     int start = lx->pos;
 43 |     while (1) {
 44 |         char c = lx->src[lx->pos];
 45 |         if (!c) break;
 46 |         if ((c >= '0' && c <= '9') || c=='+' || c=='-' || c=='.' || c=='e' || c=='E') {
 47 |             lx->pos++;
 48 |         } else {
 49 |             break;
 50 |         }
 51 |     }
 52 |     int length = lx->pos - start;
 53 |     t_json_token tok;
 54 |     tok.type = TOK_NUMBER;
 55 |     tok.text = substrdup(lx->src + start, length);
 56 |     return tok;
 57 | }
 58 | 
 59 | static t_json_token lex_string(t_json_lexer *lx) {
 60 |     int start = lx->pos;  // '"'の次の位置から文字列
 61 |     while (1) {
 62 |         char c = lx->src[lx->pos];
 63 |         if (c == '\0') {
 64 |             t_json_token tok = {TOK_UNKNOWN, NULL};
 65 |             return tok;
 66 |         }
 67 |         if (c == '"') {
 68 |             int length = lx->pos - start;
 69 |             t_json_token tok;
 70 |             tok.type = TOK_STRING;
 71 |             tok.text = substrdup(lx->src + start, length);
 72 |             lx->pos++; // 終了の '"' を consume
 73 |             return tok;
 74 |         }
 75 |         // （エスケープ処理などは必要に応じて追加）
 76 |         lx->pos++;
 77 |     }
 78 | }
 79 | 
 80 | t_json_token json_lexer_next(t_json_lexer *lx) {
 81 |     t_json_token tok = {TOK_UNKNOWN, NULL};
 82 |     skip_whitespace(lx);
 83 |     char c = lx->src[lx->pos];
 84 |     if (!c) {
 85 |         tok.type = TOK_END;
 86 |         return tok;
 87 |     }
 88 |     switch (c) {
 89 |         case '{':
 90 |             tok.type = TOK_LEFT_BRACE;
 91 |             tok.text = substrdup("{", 1);
 92 |             lx->pos++;
 93 |             return tok;
 94 |         case '}':
 95 |             tok.type = TOK_RIGHT_BRACE;
 96 |             tok.text = substrdup("}", 1);
 97 |             lx->pos++;
 98 |             return tok;
 99 |         case '[':
100 |             tok.type = TOK_LEFT_BRACKET;
101 |             tok.text = substrdup("[", 1);
102 |             lx->pos++;
103 |             return tok;
104 |         case ']':
105 |             tok.type = TOK_RIGHT_BRACKET;
106 |             tok.text = substrdup("]", 1);
107 |             lx->pos++;
108 |             return tok;
109 |         case ',':
110 |             tok.type = TOK_COMMA;
111 |             tok.text = substrdup(",", 1);
112 |             lx->pos++;
113 |             return tok;
114 |         case ':':
115 |             tok.type = TOK_COLON;
116 |             tok.text = substrdup(":", 1);
117 |             lx->pos++;
118 |             return tok;
119 |         case '"':
120 |             lx->pos++; // 開始の '"' をスキップ
121 |             return lex_string(lx);
122 |         default:
123 |             if ((c >= '0' && c <= '9') || c=='-' || c=='+')
124 |                 return lex_number(lx);
125 |             else {
126 |                 int start = lx->pos;
127 |                 while (isalpha((unsigned char)lx->src[lx->pos])) {
128 |                     lx->pos++;
129 |                 }
130 |                 int len = lx->pos - start;
131 |                 return keyword_or_unknown(lx->src + start, len);
132 |             }
133 |     }
134 | }
135 | 
136 | void json_token_destroy(t_json_token *tok) {
137 |     if (tok->text) {
138 |         free(tok->text);
139 |         tok->text = NULL;
140 |     }
141 | }
142 | 


--------------------------------------------------------------------------------
/json_lexer.h:
--------------------------------------------------------------------------------
 1 | #ifndef JSON_LEXER_H
 2 | #define JSON_LEXER_H
 3 | 
 4 | #include <stdio.h>
 5 | 
 6 | typedef enum {
 7 |     TOK_END,
 8 |     TOK_LEFT_BRACE,    // {
 9 |     TOK_RIGHT_BRACE,   // }
10 |     TOK_LEFT_BRACKET,  // [
11 |     TOK_RIGHT_BRACKET, // ]
12 |     TOK_COMMA,         // ,
13 |     TOK_COLON,         // :
14 |     TOK_STRING,        // "..."
15 |     TOK_NUMBER,        // 数値リテラル
16 |     TOK_TRUE,
17 |     TOK_FALSE,
18 |     TOK_NULL,
19 |     TOK_UNKNOWN
20 | } t_json_token_type;
21 | 
22 | typedef struct {
23 |     t_json_token_type type;
24 |     char *text;  // トークン文字列（数値や文字列リテラルなど）
25 | } t_json_token;
26 | 
27 | typedef struct {
28 |     const char *src;   // 入力文字列
29 |     int pos;           // 現在の読み取り位置
30 | } t_json_lexer;
31 | 
32 | void json_lexer_init(t_json_lexer *lx, const char *src);
33 | t_json_token json_lexer_next(t_json_lexer *lx);
34 | void json_token_destroy(t_json_token *tok);
35 | 
36 | #endif /* JSON_LEXER_H */
37 | 


--------------------------------------------------------------------------------
/json_parser_stack.c:
--------------------------------------------------------------------------------
  1 | /* json_parser_stack.c */
  2 | #include "json_parser_stack.h"
  3 | #include "json_value.h"
  4 | #include "json_lexer.h"
  5 | #include "debug.h"
  6 | #include <stdio.h>
  7 | #include <stdlib.h>
  8 | #include <string.h>
  9 | #include <assert.h>
 10 | 
 11 | /*--------------------------
 12 |    state stack 操作
 13 | --------------------------*/
 14 | static void push_state(t_json_parser_stack *ps, t_parse_state st) {
 15 |     ps->state_stack[++ps->state_top] = st;
 16 | }
 17 | static t_parse_state pop_state(t_json_parser_stack *ps) {
 18 |     return ps->state_stack[ps->state_top--];
 19 | }
 20 | static t_parse_state top_state(t_json_parser_stack *ps) {
 21 |     return ps->state_stack[ps->state_top];
 22 | }
 23 | 
 24 | /*--------------------------
 25 |    value stack 操作
 26 | --------------------------*/
 27 | static void push_value(t_json_parser_stack *ps, t_json_value *val) {
 28 |     ps->value_stack[++ps->value_top] = val;
 29 | }
 30 | static t_json_value* pop_value(t_json_parser_stack *ps) {
 31 |     return ps->value_stack[ps->value_top--];
 32 | }
 33 | static t_json_value* top_value(t_json_parser_stack *ps) {
 34 |     return ps->value_stack[ps->value_top];
 35 | }
 36 | 
 37 | /*--------------------------
 38 |    トークンを1つ進める
 39 | --------------------------*/
 40 | static void json_advance(t_json_parser_stack *ps) {
 41 |     json_token_destroy(&ps->current);
 42 |     ps->current = json_lexer_next(&ps->lexer);
 43 | #if 1
 44 |     {
 45 |         const char *tn = "???";
 46 |         switch(ps->current.type) {
 47 |             case TOK_END: tn = "END"; break;
 48 |             case TOK_LEFT_BRACE: tn = "{"; break;
 49 |             case TOK_RIGHT_BRACE: tn = "}"; break;
 50 |             case TOK_LEFT_BRACKET: tn = "["; break;
 51 |             case TOK_RIGHT_BRACKET: tn = "]"; break;
 52 |             case TOK_COMMA: tn = ","; break;
 53 |             case TOK_COLON: tn = ":"; break;
 54 |             case TOK_STRING: tn = "STRING"; break;
 55 |             case TOK_NUMBER: tn = "NUMBER"; break;
 56 |             case TOK_TRUE: tn = "TRUE"; break;
 57 |             case TOK_FALSE: tn = "FALSE"; break;
 58 |             case TOK_NULL: tn = "NULL"; break;
 59 |             case TOK_UNKNOWN: tn = "UNKNOWN"; break;
 60 |         }
 61 |         printf("Token => %s [%s]\n", tn, ps->current.text ? ps->current.text : "(null)");
 62 |     }
 63 | #endif
 64 | }
 65 | 
 66 | /*--------------------------
 67 |    forward 宣言
 68 | --------------------------*/
 69 | static void handle_token(t_json_parser_stack *ps);
 70 | static void insert_object_keyvalue(t_json_parser_stack *ps);
 71 | static void insert_array_value(t_json_parser_stack *ps);
 72 | 
 73 | /*--------------------------
 74 |    JSON パーサ初期化
 75 | --------------------------*/
 76 | void json_parser_stack_init(t_json_parser_stack *ps, const char *src) {
 77 |     memset(ps, 0, sizeof(*ps));
 78 |     json_lexer_init(&ps->lexer, src);
 79 |     ps->state_top = -1;
 80 |     ps->value_top = -1;
 81 |     ps->had_error = 0;
 82 |     ps->current = json_lexer_next(&ps->lexer);
 83 |     push_state(ps, STATE_VALUE);
 84 | }
 85 | 
 86 | /*--------------------------
 87 |    JSON パーサ終了処理
 88 | --------------------------*/
 89 | void json_parser_stack_destroy(t_json_parser_stack *ps) {
 90 |     json_token_destroy(&ps->current);
 91 |     while(ps->value_top >= 0) {
 92 |         t_json_value *v = pop_value(ps);
 93 |         json_value_free(v);
 94 |     }
 95 | }
 96 | 
 97 | /*--------------------------
 98 |    メインの parse
 99 | --------------------------*/
100 | t_json_value* json_stack_parse(t_json_parser_stack *ps) {
101 |     while (!ps->had_error) {
102 |         t_parse_state st = top_state(ps);
103 |         if (st == STATE_END)
104 |             break;
105 | 		if (ps->current.type == TOK_END) {
106 |             // もしトップ状態が STATE_EOF なら正常終了とする
107 |             if (st == STATE_EOF) {
108 |                 pop_state(ps);          // STATE_EOFを消費
109 |                 push_state(ps, STATE_END); // 最終状態に遷移
110 |                 break;
111 |             }
112 |             // それ以外の場合はエラー
113 |             ps->had_error = 1;
114 |             break;
115 |         }
116 | 			printf("\n[Before handle_token]\n");
117 | 			dump_state_stack(ps);
118 | 			dump_value_stack(ps);
119 | 			dump_current_token(ps);
120 | 			handle_token(ps);
121 | 			dump_current_token(ps);printf("\n[After handle_token]\n");
122 | 			dump_state_stack(ps);
123 | 			dump_value_stack(ps);
124 | 			dump_current_token(ps);
125 |     }
126 |     if (ps->had_error || ps->value_top != 0) {
127 |         return NULL;
128 |     }
129 | 	// ps->had_error = 0;
130 |     return pop_value(ps);
131 | }
132 | 
133 | /*--------------------------
134 |    状態＋トークンに基づく遷移処理
135 | --------------------------*/
136 | 
137 | static void handle_token(t_json_parser_stack *ps) {
138 | 
139 |     t_parse_state st = top_state(ps);
140 |     t_json_token *tk = &ps->current;
141 |     switch (st) {
142 |     case STATE_VALUE:
143 |         switch (tk->type) {
144 |         case TOK_LEFT_BRACE: {
145 |             t_json_value *objv = json_value_new(JSON_OBJECT);
146 |             objv->u.object_value = calloc(1, sizeof(t_json_object));
147 |             push_value(ps, objv);
148 |             pop_state(ps);  // STATE_VALUE解除
149 |             push_state(ps, STATE_EOF);
150 |             push_state(ps, STATE_OBJECT_KEY_OR_END);
151 |             json_advance(ps); // consume '{'
152 |             return;
153 |         }
154 |         case TOK_LEFT_BRACKET: {
155 |             t_json_value *arrv = json_value_new(JSON_ARRAY);
156 |             arrv->u.array_value = calloc(1, sizeof(t_json_array));
157 |             push_value(ps, arrv);
158 |             pop_state(ps); // STATE_VALUE解除
159 |             push_state(ps, STATE_EOF);
160 |             push_state(ps, STATE_ARRAY_VALUE_OR_END);
161 |             json_advance(ps); // consume '['
162 |             return;
163 |         }
164 |         case TOK_STRING: {
165 |             t_json_value *strv = json_value_new(JSON_STRING);
166 |             strv->u.string_value = strdup(tk->text);
167 |             push_value(ps, strv);
168 |             pop_state(ps);
169 |             push_state(ps, STATE_END);
170 |             json_advance(ps);
171 |             return;
172 |         }
173 |         case TOK_NUMBER: {
174 |             double d = strtod(tk->text, NULL);
175 |             t_json_value *nv = json_value_new(JSON_NUMBER);
176 |             nv->u.number_value = d;
177 |             push_value(ps, nv);
178 |             pop_state(ps);
179 |             push_state(ps, STATE_END);
180 |             json_advance(ps);
181 |             return;
182 |         }
183 |         case TOK_TRUE: {
184 |             t_json_value *bv = json_value_new(JSON_BOOL);
185 |             bv->u.bool_value = true;
186 |             push_value(ps, bv);
187 |             pop_state(ps);
188 |             push_state(ps, STATE_END);
189 |             json_advance(ps);
190 |             return;
191 |         }
192 |         case TOK_FALSE: {
193 |             t_json_value *bv = json_value_new(JSON_BOOL);
194 |             bv->u.bool_value = false;
195 |             push_value(ps, bv);
196 |             pop_state(ps);
197 |             push_state(ps, STATE_END);
198 |             json_advance(ps);
199 |             return;
200 |         }
201 |         case TOK_NULL: {
202 |             t_json_value *nv = json_value_new(JSON_NULL);
203 |             push_value(ps, nv);
204 |             pop_state(ps);
205 |             push_state(ps, STATE_END);
206 |             json_advance(ps);
207 |             return;
208 |         }
209 |         default:
210 |             ps->had_error = 1;
211 |             return;
212 |         }
213 |         break;
214 |     case STATE_EOF:
215 |         if (tk->type == TOK_END) {
216 |             pop_state(ps);
217 |             push_state(ps, STATE_END);
218 |         } else {
219 |             ps->had_error = 1;
220 |         }
221 |         return;
222 |     case STATE_OBJECT_KEY_OR_END:
223 | 		if (tk->type == TOK_COMMA)
224 | 		{
225 | 			json_advance(ps); //consume ,
226 | 			if (tk->type == TOK_RIGHT_BRACE)
227 | 				ps->had_error = 1;
228 | 		}
229 |         if (tk->type == TOK_RIGHT_BRACE) {
230 |             json_advance(ps); // consume '}'
231 |             pop_state(ps); // 終了状態を pop
232 |             return;
233 |         } else if (tk->type == TOK_STRING) {
234 |             t_json_value *keyv = json_value_new(JSON_STRING);
235 |             keyv->u.string_value = strdup(tk->text);
236 |             push_value(ps, keyv);
237 |             pop_state(ps); 
238 |             push_state(ps, STATE_OBJECT_COLON);
239 |             json_advance(ps); // consume key string
240 |         } else {
241 |             ps->had_error = 1;
242 |         }
243 |         return;
244 |     case STATE_OBJECT_COLON:
245 |         if (tk->type == TOK_COLON) {
246 |             pop_state(ps);
247 |             push_state(ps, STATE_OBJECT_VALUE);
248 |             json_advance(ps); // consume ':'
249 |         } else {
250 |             ps->had_error = 1;
251 |         }
252 |         return;
253 |     case STATE_OBJECT_VALUE:
254 |         switch (tk->type) {
255 |         case TOK_LEFT_BRACE: {
256 |             t_json_value *obj = json_value_new(JSON_OBJECT);
257 |             obj->u.object_value = calloc(1, sizeof(t_json_object));
258 |             push_value(ps, obj);
259 |             pop_state(ps); // STATE_OBJECT_VALUE解除
260 |             push_state(ps, STATE_OBJECT_NEXT_OR_END);
261 |             push_state(ps, STATE_OBJECT_KEY_OR_END);
262 |             json_advance(ps); // consume '{'
263 |             return;
264 |         }
265 |         case TOK_LEFT_BRACKET: {
266 |             t_json_value *arr = json_value_new(JSON_ARRAY);
267 |             arr->u.array_value = calloc(1, sizeof(t_json_array));
268 |             push_value(ps, arr);
269 |             pop_state(ps);
270 |             push_state(ps, STATE_OBJECT_NEXT_OR_END);
271 |             push_state(ps, STATE_ARRAY_VALUE_OR_END);
272 |             json_advance(ps); // consume '['
273 |             return;
274 |         }
275 |         case TOK_STRING: {
276 |             t_json_value *sv = json_value_new(JSON_STRING);
277 |             sv->u.string_value = strdup(tk->text);
278 |             push_value(ps, sv);
279 |             pop_state(ps);
280 |             push_state(ps, STATE_OBJECT_NEXT_OR_END);
281 |             json_advance(ps);
282 |             return;
283 |         }
284 |         case TOK_NUMBER: {
285 |             double d = strtod(tk->text, NULL);
286 |             t_json_value *nv = json_value_new(JSON_NUMBER);
287 |             nv->u.number_value = d;
288 |             push_value(ps, nv);
289 |             pop_state(ps);
290 |             push_state(ps, STATE_OBJECT_NEXT_OR_END);
291 |             json_advance(ps);
292 |             return;
293 |         }
294 |         case TOK_TRUE: {
295 |             t_json_value *bv = json_value_new(JSON_BOOL);
296 |             bv->u.bool_value = true;
297 |             push_value(ps, bv);
298 |             pop_state(ps);
299 |             push_state(ps, STATE_OBJECT_NEXT_OR_END);
300 |             json_advance(ps);
301 |             return;
302 |         }
303 |         case TOK_FALSE: {
304 |             t_json_value *bv = json_value_new(JSON_BOOL);
305 |             bv->u.bool_value = false;
306 |             push_value(ps, bv);
307 |             pop_state(ps);
308 |             push_state(ps, STATE_OBJECT_NEXT_OR_END);
309 |             json_advance(ps);
310 |             return;
311 |         }
312 |         case TOK_NULL: {
313 |             t_json_value *nv = json_value_new(JSON_NULL);
314 |             push_value(ps, nv);
315 |             pop_state(ps);
316 |             push_state(ps, STATE_OBJECT_NEXT_OR_END);
317 |             json_advance(ps);
318 |             return;
319 |         }
320 |         default:
321 |             ps->had_error = 1;
322 |             return;
323 |         }
324 |         break;
325 |     case STATE_OBJECT_NEXT_OR_END: {
326 |             insert_object_keyvalue(ps);
327 |             /* ここで次のトークンをチェック */
328 |             if (ps->current.type == TOK_COMMA) {
329 |                 // json_advance(ps); // consume ','
330 | 				pop_state(ps);
331 |                 push_state(ps, STATE_OBJECT_KEY_OR_END);
332 |             } else if (ps->current.type == TOK_RIGHT_BRACE) {
333 |                 json_advance(ps); // consume '}'
334 |                 pop_state(ps);    // STATE_OBJECT_NEXT_OR_ENDをpop
335 |             } else {
336 |                 ps->had_error = 1;
337 |             }
338 |             return;
339 |         }
340 |     case STATE_ARRAY_VALUE_OR_END:
341 |         if (tk->type == TOK_RIGHT_BRACKET) {
342 |             pop_state(ps);
343 |             json_advance(ps); // consume ']'
344 |         } else {
345 |             pop_state(ps);
346 |             // push_state(ps, STATE_ARRAY_NEXT_OR_END);
347 |             push_state(ps, STATE_ARRAY_VALUE);
348 |         }
349 |         return;
350 |     case STATE_ARRAY_VALUE:
351 |         switch (tk->type) {
352 |         case TOK_LEFT_BRACE: {
353 |             t_json_value *obj = json_value_new(JSON_OBJECT);
354 |             obj->u.object_value = calloc(1, sizeof(t_json_object));
355 |             push_value(ps, obj);
356 |             pop_state(ps);
357 |             push_state(ps, STATE_ARRAY_NEXT_OR_END);
358 |             push_state(ps, STATE_OBJECT_KEY_OR_END);
359 |             json_advance(ps); // consume '{'
360 |             return;
361 |         }
362 |         case TOK_LEFT_BRACKET: {
363 |             t_json_value *arr = json_value_new(JSON_ARRAY);
364 |             arr->u.array_value = calloc(1, sizeof(t_json_array));
365 |             push_value(ps, arr);
366 |             pop_state(ps);
367 |             push_state(ps, STATE_ARRAY_NEXT_OR_END);
368 |             push_state(ps, STATE_ARRAY_VALUE_OR_END);
369 |             json_advance(ps); // consume '['
370 |             return;
371 |         }
372 |         case TOK_STRING: {
373 |             t_json_value *sv = json_value_new(JSON_STRING);
374 |             sv->u.string_value = strdup(tk->text);
375 |             push_value(ps, sv);
376 |             pop_state(ps);
377 |             push_state(ps, STATE_ARRAY_NEXT_OR_END);
378 |             json_advance(ps);
379 |             return;
380 |         }
381 |         case TOK_NUMBER: {
382 |             double d = strtod(tk->text, NULL);
383 |             t_json_value *nv = json_value_new(JSON_NUMBER);
384 |             nv->u.number_value = d;
385 |             push_value(ps, nv);
386 |             pop_state(ps);
387 |             push_state(ps, STATE_ARRAY_NEXT_OR_END);
388 |             json_advance(ps);
389 |             return;
390 |         }
391 |         case TOK_TRUE: {
392 |             t_json_value *bv = json_value_new(JSON_BOOL);
393 |             bv->u.bool_value = true;
394 |             push_value(ps, bv);
395 |             pop_state(ps);
396 |             push_state(ps, STATE_ARRAY_NEXT_OR_END);
397 |             json_advance(ps);
398 |             return;
399 |         }
400 |         case TOK_FALSE: {
401 |             t_json_value *bv = json_value_new(JSON_BOOL);
402 |             bv->u.bool_value = false;
403 |             push_value(ps, bv);
404 |             pop_state(ps);
405 |             push_state(ps, STATE_ARRAY_NEXT_OR_END);
406 |             json_advance(ps);
407 |             return;
408 |         }
409 |         case TOK_NULL: {
410 |             t_json_value *nv = json_value_new(JSON_NULL);
411 |             push_value(ps, nv);
412 |             pop_state(ps);
413 |             push_state(ps, STATE_ARRAY_NEXT_OR_END);
414 |             json_advance(ps);
415 |             return;
416 |         }
417 |         default:
418 |             ps->had_error = 1;
419 |             return;
420 |         }
421 |         break;
422 |     case STATE_ARRAY_NEXT_OR_END: {
423 |             insert_array_value(ps);
424 |             if (ps->current.type == TOK_COMMA) {
425 |                 json_advance(ps); // consume ','
426 | 				pop_state(ps);
427 |                 push_state(ps, STATE_ARRAY_VALUE);
428 |             } else if (ps->current.type == TOK_RIGHT_BRACKET) {
429 |                 json_advance(ps); // consume ']'
430 |                 pop_state(ps);    // pop STATE_ARRAY_NEXT_OR_END
431 |             } else {
432 |                 ps->had_error = 1;
433 |             }
434 |             return;
435 |         }
436 |     case STATE_END:
437 |     case STATE_INVALID:
438 |         ps->had_error = 1;
439 |         return;
440 |     }
441 | }
442 | 
443 | /*--------------------------------
444 |    insert_object_keyvalue:
445 |    スタック上から (value, key, object) を取り出し、
446 |    オブジェクトに key/value ペアを登録する。
447 | --------------------------------*/
448 | static void insert_object_keyvalue(t_json_parser_stack *ps) {
449 |     t_json_value *val = pop_value(ps);
450 |     t_json_value *keyv = pop_value(ps);
451 |     t_json_value *objv = pop_value(ps);
452 |     if (!objv || !keyv || !val) {
453 |         ps->had_error = 1;
454 |         return;
455 |     }
456 |     if (objv->type != JSON_OBJECT || keyv->type != JSON_STRING) {
457 |         ps->had_error = 1;
458 |         return;
459 |     }
460 |     json_object_put(objv->u.object_value, keyv->u.string_value, val);
461 |     json_value_free(keyv);
462 |     push_value(ps, objv);
463 | }
464 | 
465 | /*--------------------------------
466 |    insert_array_value:
467 |    スタック上から (value, array) を取り出し、
468 |    配列に値を追加する。
469 | --------------------------------*/
470 | static void insert_array_value(t_json_parser_stack *ps) {
471 |     t_json_value *val = pop_value(ps);
472 |     t_json_value *arrv = pop_value(ps);
473 |     if (!arrv || !val) {
474 |         ps->had_error = 1;
475 |         return;
476 |     }
477 |     if (arrv->type != JSON_ARRAY) {
478 |         ps->had_error = 1;
479 |         return;
480 |     }
481 |     json_array_add(arrv->u.array_value, val);
482 |     push_value(ps, arrv);
483 | }
484 | 


--------------------------------------------------------------------------------
/json_parser_stack.h:
--------------------------------------------------------------------------------
 1 | #ifndef JSON_PARSER_STACK_H
 2 | #define JSON_PARSER_STACK_H
 3 | 
 4 | #include "json_lexer.h"
 5 | #include "json_value.h"
 6 | 
 7 | typedef enum {
 8 |     STATE_VALUE,
 9 |     STATE_OBJECT_KEY_OR_END,
10 |     STATE_OBJECT_COLON,
11 |     STATE_OBJECT_VALUE,
12 |     STATE_OBJECT_NEXT_OR_END,
13 |     STATE_ARRAY_VALUE_OR_END,
14 |     STATE_ARRAY_VALUE,
15 |     STATE_ARRAY_NEXT_OR_END,
16 |     STATE_EOF,
17 |     STATE_END,
18 |     STATE_INVALID
19 | } t_parse_state;
20 | 
21 | #define STACK_SIZE 256
22 | 
23 | typedef struct {
24 |     t_json_lexer lexer;
25 |     t_json_token current;
26 |     t_parse_state state_stack[STACK_SIZE];
27 |     int state_top;
28 |     t_json_value* value_stack[STACK_SIZE];
29 |     int value_top;
30 |     int had_error;
31 | } t_json_parser_stack;
32 | 
33 | void json_parser_stack_init(t_json_parser_stack *ps, const char *src);
34 | void json_parser_stack_destroy(t_json_parser_stack *ps);
35 | t_json_value* json_stack_parse(t_json_parser_stack *ps);
36 | 
37 | #endif /* JSON_PARSER_STACK_H */
38 | 


--------------------------------------------------------------------------------
/json_value.c:
--------------------------------------------------------------------------------
 1 | #include "json_value.h"
 2 | #include <stdlib.h>
 3 | #include <string.h>
 4 | #include <stdio.h>
 5 | 
 6 | t_json_value* json_value_new(t_json_type type) {
 7 |     t_json_value *v = (t_json_value*)calloc(1, sizeof(t_json_value));
 8 |     if (!v) return NULL;
 9 |     v->type = type;
10 |     return v;
11 | }
12 | 
13 | void json_value_free(t_json_value *val) {
14 |     if (!val) return;
15 |     switch (val->type) {
16 |         case JSON_STRING:
17 |             free(val->u.string_value);
18 |             break;
19 |         case JSON_ARRAY:
20 |             if (val->u.array_value) {
21 |                 for (int i = 0; i < val->u.array_value->size; i++) {
22 |                     json_value_free(val->u.array_value->items[i]);
23 |                 }
24 |                 free(val->u.array_value->items);
25 |                 free(val->u.array_value);
26 |             }
27 |             break;
28 |         case JSON_OBJECT:
29 |             if (val->u.object_value) {
30 |                 for (int i = 0; i < val->u.object_value->size; i++) {
31 |                     free(val->u.object_value->pairs[i].key);
32 |                     json_value_free(val->u.object_value->pairs[i].value);
33 |                 }
34 |                 free(val->u.object_value->pairs);
35 |                 free(val->u.object_value);
36 |             }
37 |             break;
38 |         default:
39 |             break;
40 |     }
41 |     free(val);
42 | }
43 | 
44 | void json_array_add(t_json_array *arr, t_json_value *item) {
45 |     if (!arr) return;
46 |     if (arr->size >= arr->capacity) {
47 |         int newcap = (arr->capacity < 4) ? 4 : arr->capacity * 2;
48 |         t_json_value **newitems = realloc(arr->items, sizeof(t_json_value*) * newcap);
49 |         if (!newitems) {
50 |             fprintf(stderr, "json_array_add: realloc failed\n");
51 |             return;
52 |         }
53 |         arr->items = newitems;
54 |         arr->capacity = newcap;
55 |     }
56 |     arr->items[arr->size++] = item;
57 | }
58 | 
59 | void json_object_put(t_json_object *obj, const char *key, t_json_value *val) {
60 |     if (!obj) return;
61 |     if (obj->size >= obj->capacity) {
62 |         int newcap = (obj->capacity < 4) ? 4 : obj->capacity * 2;
63 |         t_json_keyvalue *newpairs = realloc(obj->pairs, sizeof(t_json_keyvalue) * newcap);
64 |         if (!newpairs) {
65 |             fprintf(stderr, "json_object_put: realloc failed\n");
66 |             return;
67 |         }
68 |         obj->pairs = newpairs;
69 |         obj->capacity = newcap;
70 |     }
71 |     obj->pairs[obj->size].key = strdup(key);
72 |     obj->pairs[obj->size].value = val;
73 |     obj->size++;
74 | }
75 | 


--------------------------------------------------------------------------------
/json_value.h:
--------------------------------------------------------------------------------
 1 | #ifndef JSON_VALUE_H
 2 | #define JSON_VALUE_H
 3 | 
 4 | #include <stdbool.h>
 5 | 
 6 | // JSONの型を表す
 7 | typedef enum e_json_type {
 8 |     JSON_NULL,
 9 |     JSON_BOOL,
10 |     JSON_NUMBER,
11 |     JSON_STRING,
12 |     JSON_ARRAY,
13 |     JSON_OBJECT
14 | } t_json_type;
15 | 
16 | // 前方宣言
17 | struct s_json_array;
18 | struct s_json_object;
19 | 
20 | // JSONの値を格納するための構造
21 | typedef struct s_json_value {
22 |     t_json_type type;
23 |     union {
24 |         bool bool_value;
25 |         double number_value;
26 |         char *string_value;
27 |         struct s_json_array *array_value;
28 |         struct s_json_object *object_value;
29 |     } u;
30 | } t_json_value;
31 | 
32 | // 配列の定義
33 | typedef struct s_json_array {
34 |     t_json_value **items;
35 |     int size;
36 |     int capacity;
37 | } t_json_array;
38 | 
39 | // キーと値のペア
40 | typedef struct s_json_keyvalue {
41 |     char *key;
42 |     t_json_value *value;
43 | } t_json_keyvalue;
44 | 
45 | // オブジェクトの定義
46 | typedef struct s_json_object {
47 |     t_json_keyvalue *pairs;
48 |     int size;
49 |     int capacity;
50 | } t_json_object;
51 | 
52 | #ifdef __cplusplus
53 | extern "C" {
54 | #endif
55 | 
56 | /** 
57 |  * 新規に t_json_value* を type指定で作成
58 |  * メンバはゼロ初期化される 
59 |  */
60 | t_json_value* json_value_new(t_json_type type);
61 | 
62 | /** 値を再帰的に free する */
63 | void json_value_free(t_json_value *val);
64 | 
65 | /** 配列に要素を追加する */
66 | void json_array_add(t_json_array *arr, t_json_value *item);
67 | 
68 | /** オブジェクトにキーと値を追加する */
69 | void json_object_put(t_json_object *obj, const char *key, t_json_value *val);
70 | 
71 | #ifdef __cplusplus
72 | }
73 | #endif
74 | 
75 | #endif /* JSON_VALUE_H */
76 | 


--------------------------------------------------------------------------------
/main.c:
--------------------------------------------------------------------------------
 1 | #include "json_parser_stack.h"
 2 | #include "json_value.h"
 3 | #include <stdio.h>
 4 | #include <stdlib.h>
 5 | 
 6 | static void dump_value(const t_json_value *v, int indent);
 7 | static void print_indent(int indent) {
 8 |     for (int i = 0; i < indent; i++) {
 9 |         putchar(' ');
10 |     }
11 | }
12 | static void dump_array(const t_json_array *arr, int indent) {
13 |     printf("[\n");
14 |     for (int i = 0; i < arr->size; i++) {
15 |         print_indent(indent + 2);
16 |         dump_value(arr->items[i], indent + 2);
17 |         if (i < arr->size - 1) printf(",");
18 |         printf("\n");
19 |     }
20 |     print_indent(indent);
21 |     printf("]");
22 | }
23 | static void dump_object(const t_json_object *obj, int indent) {
24 |     printf("{\n");
25 |     for (int i = 0; i < obj->size; i++) {
26 |         print_indent(indent + 2);
27 |         printf("\"%s\": ", obj->pairs[i].key);
28 |         dump_value(obj->pairs[i].value, indent + 2);
29 |         if (i < obj->size - 1) printf(",");
30 |         printf("\n");
31 |     }
32 |     print_indent(indent);
33 |     printf("}");
34 | }
35 | static void dump_value(const t_json_value *v, int indent) {
36 |     if (!v) {
37 |         printf("null");
38 |         return;
39 |     }
40 |     switch(v->type) {
41 |         case JSON_NULL:
42 |             printf("null");
43 |             break;
44 |         case JSON_BOOL:
45 |             printf(v->u.bool_value ? "true" : "false");
46 |             break;
47 |         case JSON_NUMBER:
48 |             printf("%g", v->u.number_value);
49 |             break;
50 |         case JSON_STRING:
51 |             printf("\"%s\"", v->u.string_value);
52 |             break;
53 |         case JSON_ARRAY:
54 |             dump_array(v->u.array_value, indent);
55 |             break;
56 |         case JSON_OBJECT:
57 |             dump_object(v->u.object_value, indent);
58 |             break;
59 |     }
60 | }
61 | 
62 | int main(int argc, char **argv) {
63 |     if (argc < 2) {
64 |         fprintf(stderr, "Usage: %s <jsonfile>\n", argv[0]);
65 |         return 1;
66 |     }
67 |     FILE *fp = fopen(argv[1], "rb");
68 |     if (!fp) {
69 |         perror("fopen");
70 |         return 1;
71 |     }
72 |     fseek(fp, 0, SEEK_END);
73 |     long sz = ftell(fp);
74 |     fseek(fp, 0, SEEK_SET);
75 |     char *buf = malloc(sz + 1);
76 |     if (!buf) {
77 |         fclose(fp);
78 |         return 1;
79 |     }
80 |     fread(buf, 1, sz, fp);
81 |     buf[sz] = '\0';
82 |     fclose(fp);
83 | 
84 |     t_json_parser_stack parser;
85 |     json_parser_stack_init(&parser, buf);
86 |     t_json_value *root = json_stack_parse(&parser);
87 |     if (!root || parser.had_error) {
88 |         printf("Parse error.\n");
89 |     } else {
90 |         dump_value(root, 0);
91 |         printf("\n");
92 |         json_value_free(root);
93 |     }
94 |     json_parser_stack_destroy(&parser);
95 |     free(buf);
96 |     return 0;
97 | }
98 | 


--------------------------------------------------------------------------------
/out:
--------------------------------------------------------------------------------
  1 | 
  2 | [Before handle_token]
  3 | State Stack (top -> bottom):
  4 |   [0]: STATE_VALUE
  5 | Value Stack (top -> bottom):
  6 | Current Token: { [{]
  7 | Token => STRING [key]
  8 | Current Token: STRING [key]
  9 | 
 10 | [After handle_token]
 11 | State Stack (top -> bottom):
 12 |   [1]: STATE_OBJECT_KEY_OR_END
 13 |   [0]: STATE_EOF
 14 | Value Stack (top -> bottom):
 15 |   [0]: JSON_OBJECT
 16 | Current Token: STRING [key]
 17 | 
 18 | [Before handle_token]
 19 | State Stack (top -> bottom):
 20 |   [1]: STATE_OBJECT_KEY_OR_END
 21 |   [0]: STATE_EOF
 22 | Value Stack (top -> bottom):
 23 |   [0]: JSON_OBJECT
 24 | Current Token: STRING [key]
 25 | Token => : [:]
 26 | Current Token: : [:]
 27 | 
 28 | [After handle_token]
 29 | State Stack (top -> bottom):
 30 |   [1]: STATE_OBJECT_COLON
 31 |   [0]: STATE_EOF
 32 | Value Stack (top -> bottom):
 33 |   [1]: JSON_STRING ("key")
 34 |   [0]: JSON_OBJECT
 35 | Current Token: : [:]
 36 | 
 37 | [Before handle_token]
 38 | State Stack (top -> bottom):
 39 |   [1]: STATE_OBJECT_COLON
 40 |   [0]: STATE_EOF
 41 | Value Stack (top -> bottom):
 42 |   [1]: JSON_STRING ("key")
 43 |   [0]: JSON_OBJECT
 44 | Current Token: : [:]
 45 | Token => [ [[]
 46 | Current Token: [ [[]
 47 | 
 48 | [After handle_token]
 49 | State Stack (top -> bottom):
 50 |   [1]: STATE_OBJECT_VALUE
 51 |   [0]: STATE_EOF
 52 | Value Stack (top -> bottom):
 53 |   [1]: JSON_STRING ("key")
 54 |   [0]: JSON_OBJECT
 55 | Current Token: [ [[]
 56 | 
 57 | [Before handle_token]
 58 | State Stack (top -> bottom):
 59 |   [1]: STATE_OBJECT_VALUE
 60 |   [0]: STATE_EOF
 61 | Value Stack (top -> bottom):
 62 |   [1]: JSON_STRING ("key")
 63 |   [0]: JSON_OBJECT
 64 | Current Token: [ [[]
 65 | Token => NUMBER [2]
 66 | Current Token: NUMBER [2]
 67 | 
 68 | [After handle_token]
 69 | State Stack (top -> bottom):
 70 |   [2]: STATE_ARRAY_VALUE_OR_END
 71 |   [1]: STATE_OBJECT_NEXT_OR_END
 72 |   [0]: STATE_EOF
 73 | Value Stack (top -> bottom):
 74 |   [2]: JSON_ARRAY
 75 |   [1]: JSON_STRING ("key")
 76 |   [0]: JSON_OBJECT
 77 | Current Token: NUMBER [2]
 78 | 
 79 | [Before handle_token]
 80 | State Stack (top -> bottom):
 81 |   [2]: STATE_ARRAY_VALUE_OR_END
 82 |   [1]: STATE_OBJECT_NEXT_OR_END
 83 |   [0]: STATE_EOF
 84 | Value Stack (top -> bottom):
 85 |   [2]: JSON_ARRAY
 86 |   [1]: JSON_STRING ("key")
 87 |   [0]: JSON_OBJECT
 88 | Current Token: NUMBER [2]
 89 | Current Token: NUMBER [2]
 90 | 
 91 | [After handle_token]
 92 | State Stack (top -> bottom):
 93 |   [3]: STATE_ARRAY_VALUE
 94 |   [2]: STATE_ARRAY_NEXT_OR_END
 95 |   [1]: STATE_OBJECT_NEXT_OR_END
 96 |   [0]: STATE_EOF
 97 | Value Stack (top -> bottom):
 98 |   [2]: JSON_ARRAY
 99 |   [1]: JSON_STRING ("key")
100 |   [0]: JSON_OBJECT
101 | Current Token: NUMBER [2]
102 | 
103 | [Before handle_token]
104 | State Stack (top -> bottom):
105 |   [3]: STATE_ARRAY_VALUE
106 |   [2]: STATE_ARRAY_NEXT_OR_END
107 |   [1]: STATE_OBJECT_NEXT_OR_END
108 |   [0]: STATE_EOF
109 | Value Stack (top -> bottom):
110 |   [2]: JSON_ARRAY
111 |   [1]: JSON_STRING ("key")
112 |   [0]: JSON_OBJECT
113 | Current Token: NUMBER [2]
114 | Token => , [,]
115 | Current Token: , [,]
116 | 
117 | [After handle_token]
118 | State Stack (top -> bottom):
119 |   [3]: STATE_ARRAY_NEXT_OR_END
120 |   [2]: STATE_ARRAY_NEXT_OR_END
121 |   [1]: STATE_OBJECT_NEXT_OR_END
122 |   [0]: STATE_EOF
123 | Value Stack (top -> bottom):
124 |   [3]: JSON_NUMBER (2)
125 |   [2]: JSON_ARRAY
126 |   [1]: JSON_STRING ("key")
127 |   [0]: JSON_OBJECT
128 | Current Token: , [,]
129 | 
130 | [Before handle_token]
131 | State Stack (top -> bottom):
132 |   [3]: STATE_ARRAY_NEXT_OR_END
133 |   [2]: STATE_ARRAY_NEXT_OR_END
134 |   [1]: STATE_OBJECT_NEXT_OR_END
135 |   [0]: STATE_EOF
136 | Value Stack (top -> bottom):
137 |   [3]: JSON_NUMBER (2)
138 |   [2]: JSON_ARRAY
139 |   [1]: JSON_STRING ("key")
140 |   [0]: JSON_OBJECT
141 | Current Token: , [,]
142 | Token => NUMBER [3]
143 | Current Token: NUMBER [3]
144 | 
145 | [After handle_token]
146 | State Stack (top -> bottom):
147 |   [2]: STATE_ARRAY_VALUE
148 |   [1]: STATE_OBJECT_NEXT_OR_END
149 |   [0]: STATE_EOF
150 | Value Stack (top -> bottom):
151 |   [2]: JSON_ARRAY
152 |   [1]: JSON_STRING ("key")
153 |   [0]: JSON_OBJECT
154 | Current Token: NUMBER [3]
155 | 
156 | [Before handle_token]
157 | State Stack (top -> bottom):
158 |   [2]: STATE_ARRAY_VALUE
159 |   [1]: STATE_OBJECT_NEXT_OR_END
160 |   [0]: STATE_EOF
161 | Value Stack (top -> bottom):
162 |   [2]: JSON_ARRAY
163 |   [1]: JSON_STRING ("key")
164 |   [0]: JSON_OBJECT
165 | Current Token: NUMBER [3]
166 | Token => , [,]
167 | Current Token: , [,]
168 | 
169 | [After handle_token]
170 | State Stack (top -> bottom):
171 |   [2]: STATE_ARRAY_NEXT_OR_END
172 |   [1]: STATE_OBJECT_NEXT_OR_END
173 |   [0]: STATE_EOF
174 | Value Stack (top -> bottom):
175 |   [3]: JSON_NUMBER (3)
176 |   [2]: JSON_ARRAY
177 |   [1]: JSON_STRING ("key")
178 |   [0]: JSON_OBJECT
179 | Current Token: , [,]
180 | 
181 | [Before handle_token]
182 | State Stack (top -> bottom):
183 |   [2]: STATE_ARRAY_NEXT_OR_END
184 |   [1]: STATE_OBJECT_NEXT_OR_END
185 |   [0]: STATE_EOF
186 | Value Stack (top -> bottom):
187 |   [3]: JSON_NUMBER (3)
188 |   [2]: JSON_ARRAY
189 |   [1]: JSON_STRING ("key")
190 |   [0]: JSON_OBJECT
191 | Current Token: , [,]
192 | Token => NUMBER [4]
193 | Current Token: NUMBER [4]
194 | 
195 | [After handle_token]
196 | State Stack (top -> bottom):
197 |   [1]: STATE_ARRAY_VALUE
198 |   [0]: STATE_EOF
199 | Value Stack (top -> bottom):
200 |   [2]: JSON_ARRAY
201 |   [1]: JSON_STRING ("key")
202 |   [0]: JSON_OBJECT
203 | Current Token: NUMBER [4]
204 | 
205 | [Before handle_token]
206 | State Stack (top -> bottom):
207 |   [1]: STATE_ARRAY_VALUE
208 |   [0]: STATE_EOF
209 | Value Stack (top -> bottom):
210 |   [2]: JSON_ARRAY
211 |   [1]: JSON_STRING ("key")
212 |   [0]: JSON_OBJECT
213 | Current Token: NUMBER [4]
214 | Token => ] []]
215 | Current Token: ] []]
216 | 
217 | [After handle_token]
218 | State Stack (top -> bottom):
219 |   [1]: STATE_ARRAY_NEXT_OR_END
220 |   [0]: STATE_EOF
221 | Value Stack (top -> bottom):
222 |   [3]: JSON_NUMBER (4)
223 |   [2]: JSON_ARRAY
224 |   [1]: JSON_STRING ("key")
225 |   [0]: JSON_OBJECT
226 | Current Token: ] []]
227 | 
228 | [Before handle_token]
229 | State Stack (top -> bottom):
230 |   [1]: STATE_ARRAY_NEXT_OR_END
231 |   [0]: STATE_EOF
232 | Value Stack (top -> bottom):
233 |   [3]: JSON_NUMBER (4)
234 |   [2]: JSON_ARRAY
235 |   [1]: JSON_STRING ("key")
236 |   [0]: JSON_OBJECT
237 | Current Token: ] []]
238 | Token => } [}]
239 | Current Token: } [}]
240 | 
241 | [After handle_token]
242 | State Stack (top -> bottom):
243 |   [0]: STATE_EOF
244 | Value Stack (top -> bottom):
245 |   [2]: JSON_ARRAY
246 |   [1]: JSON_STRING ("key")
247 |   [0]: JSON_OBJECT
248 | Current Token: } [}]
249 | 
250 | [Before handle_token]
251 | State Stack (top -> bottom):
252 |   [0]: STATE_EOF
253 | Value Stack (top -> bottom):
254 |   [2]: JSON_ARRAY
255 |   [1]: JSON_STRING ("key")
256 |   [0]: JSON_OBJECT
257 | Current Token: } [}]
258 | Current Token: } [}]
259 | 
260 | [After handle_token]
261 | State Stack (top -> bottom):
262 |   [0]: STATE_EOF
263 | Value Stack (top -> bottom):
264 |   [2]: JSON_ARRAY
265 |   [1]: JSON_STRING ("key")
266 |   [0]: JSON_OBJECT
267 | Current Token: } [}]
268 | Parse error.
269 | 


--------------------------------------------------------------------------------