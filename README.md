# JSON Parser

C言語で実装されたJSONパーサーです。table駆動方式を使用して、効率的で保守性の高いコードになっています。

## 特徴

- **table駆動方式**: switch-case文を使わず、テーブルで状態遷移を管理
- **デバッグ機能**: パース過程の詳細な可視化が可能
- **ファイル読み込み**: JSONファイルを直接読み込んで解析
- **完全なJSON対応**: オブジェクト、配列、文字列、数値、真偽値、nullをサポート

## プロジェクト構造

```
json_parser/
├── incs/                          # ヘッダーファイル
│   ├── json_lexer.h              # レキサー関連の定義
│   ├── json_parser_stack.h       # パーサースタック関連の定義
│   ├── json_value.h              # JSON値の型定義
│   ├── debug.h                   # デバッグ機能の定義
│   └── utils.h                   # ユーティリティ関数
├── srcs/                         # ソースファイル
│   ├── json_lexer.c              # レキサー実装
│   ├── json_lexer_utils.c        # レキサー補助関数
│   ├── json_lexer_utils2.c       # レキサー補助関数2
│   ├── json_parser_stack.c       # パーサースタック実装
│   ├── json_parser_utils.c       # パーサー補助関数
│   ├── json_value.c              # JSON値の操作
│   ├── json_free.c               # メモリ解放
│   ├── json_insert.c             # JSON値の挿入
│   ├── operate_stack.c           # スタック操作
│   ├── init.c                    # 初期化処理
│   ├── debug.c                   # デバッグ機能実装
│   ├── utils/                    # ユーティリティ関数群
│   └── handle_state/             # 状態処理関数群
├── main.c                        # メイン関数
├── Makefile                      # ビルド設定
├── test.json                     # テスト用JSONファイル
└── README.md                     # このファイル
```

## ビルド方法

### 通常ビルド
```bash
make
```

### デバッグビルド
```bash
make debug
```

### クリーンアップ
```bash
make clean      # オブジェクトファイル削除
make fclean     # オブジェクトファイルと実行ファイル削除
make re         # 完全リビルド
```

## 使用方法

### 基本的な使用方法
```bash
./json_parser
```

このコマンドは`test.json`ファイルを読み込んで解析します。

### デバッグモードでの実行
```bash
make debug
./json_parser
```

デバッグモードでは、パース過程の詳細な情報が表示されます：
- 現在のトークン
- 状態スタックの内容（上から下へ）
- 値スタックの内容（上から下へ）

## 実装の詳細

### table駆動方式

このプロジェクトでは、switch-case文を避けてtable駆動方式を採用しています：

#### 1. パーサーテーブル (`g_parser_table`)
```c
static const t_parser_transition g_parser_table[] = {
    {STATE_VALUE, TOK_LEFT_BRACE, parse_value_left_brace},
    {STATE_VALUE, TOK_LEFT_BRACKET, parse_value_left_bracket},
    // ... 他の遷移規則
};
```

#### 2. デバッグ用テーブル
- **STATE名テーブル**: `t_state_name_table`
- **トークン名テーブル**: `t_token_name_table`
- **JSON型名テーブル**: `t_json_type_name_table`
- **値フォーマットテーブル**: `value_formatter_table`

#### 3. dump関数テーブル
```c
static const t_dump_table dump_table[] = {
    {JSON_NULL, dump_null_value, "null"},
    {JSON_BOOL, dump_bool_value, "boolean"},
    // ... 他の型
};
```

### デバッグ機能

デバッグモードでは以下の情報が表示されます：

```
=== Current state ===
Current Token: TOK_STRING("name")
State Stack (top -> bottom):
  [1]: STATE_OBJECT_KEY_OR_END
  [0]: STATE_EOF
Value Stack (top -> bottom):
  [0]: JSON_OBJECT
================
```

## サポートされているJSON形式

- **オブジェクト**: `{"key": "value"}`
- **配列**: `[1, 2, 3, "hello"]`
- **文字列**: `"example"`
- **数値**: `42`, `3.14`
- **真偽値**: `true`, `false`
- **null**: `null`
- **ネスト**: オブジェクトや配列のネスト

## テスト用JSONファイル

`test.json`には以下のような複雑なJSONが含まれています：

```json
{
  "name": "test",
  "number": 42,
  "boolean": true,
  "null_value": null,
  "array": [1, 2, 3, "hello"],
  "object": {
    "nested": "value",
    "another": 123
  }
}
```

## 技術的な特徴

### メモリ管理
- 適切なメモリ解放処理
- メモリリークの防止

### エラーハンドリング
- パースエラーの検出
- 適切なエラー状態の管理

### パフォーマンス
- 効率的なテーブル検索
- 最小限のメモリ使用量

## 開発環境

- **言語**: C言語
- **コンパイラ**: GCC
- **OS**: macOS/Linux対応
- **警告レベル**: `-Wall -Wextra -Werror`

## ライセンス

このプロジェクトは個人学習目的で作成されました。自由に使用・改変・配布可能です。

## 作者

miwasa (42 student)
