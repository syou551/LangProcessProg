# 設計概要

## モジュール概要

### main
- メイン関数

### init_scan,end_scan
- ソースコードの読み取りの初期化および終了処理

### scan
- 1つのトークンを読み取る
- tokenとして有効な文字列が現れた場合に，ループを抜けてコードを返す

```C:scan.c
int scan(){
    for(cbuf = read_char();cbuf != NULL;cbuf = read_char()){
        switch(cbuf){
            case _/*空白やタブ*/:/*読み飛ばす*/break;
            :
        }
    }
}
```

### read_char
- 1文字読み込む
- どこまで読み込んだかは外から意識しなくていいようにする

### get_linenum
- 現在読み込み処理を行っている行番号を返す(改行文字ごとにカウントアップ)
- 改行文字が`\n`だけに限らないことに注意



## ファイル構成

### main.c
- メイン関数

### parser.c
- 構文解析系の関数群

### printer.c
- プリティプリンタ系の関数群

### fileHandle.c
- ファイルポインタの処理

### counter.c
- トークンのカウント関係の処理

### scan.c
- トークン読み込みおよび認識関係の処理
