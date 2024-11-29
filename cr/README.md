# cr
## 概要
型チェックや重複宣言などを確認し，クロスリファレンス表を出力する

## 設計
### モジュール
- fileHandle.c, scan.c - スキャンモジュール
- parser.c, prettyPrint.c - 構文解析系
- idList.c - クロスリファレンス表作成系

### ファイル
- idList.c - 記号表作成のための関数群