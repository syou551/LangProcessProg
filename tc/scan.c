#include "scan.h"
#include <ctype.h>

// digit scaning flag
int digit_scan;

int scan(){
    char cbuf, prev;
    int strlen = 0;
    digit_scan = 0;
    int code = 0;

    //When code is 0, skip this letter
    for(cbuf = read_char();code <= 0;cbuf = read_char()){
        if(isspace(cbuf)){
            code = 0;
        }else if(isalpha(cbuf)){
            code = get_tokencode(0,cbuf);
        }else if(isdigit(cbuf)){
            code = get_tokencode(1,cbuf);
        }else{
            switch(cbuf){
                //comment
                case '{':
                    for(cbuf = read_char();cbuf != '}';cbuf = read_char()){};
                    code = 0;
                    break;
                case '/': 
                    cbuf = read_char();
                    if(cbuf != '*') return error("Invalid symbol array!(Comment format)");
                    prev = cbuf; 
                    for(cbuf = read_char();prev != '*' && cbuf != '/';cbuf = read_char()){
                        prev = cbuf;
                    }
                    code = 0;
                    break;
                case '\'':
                    code = check_string_format(cbuf);
                default: 
                    code = get_tokencode_symbol(cbuf);
            }
        }

    }
    return code;
}

//mode
//  0: string or keyword
//  1: num
int get_tokencode(int mode, char _cbuf){
    char cbuf = _cbuf;
    int strlen = 0;
    int maxlen = mode ? MAXSTRSIZE-1 : 6;
    if(mode == 1){
        digit_scan = 1;
    }
    for(;isalpha(cbuf);cbuf = read_char()){
        if(strlen >= maxlen) return error("Word length is too long!");
        string_attr[strlen++] = cbuf;
    }
    //put back buffer to stream
    ungetc(cbuf,fp);
    int code = check_strbuf(string_attr,strlen,mode);
    //FIXME: In case like 10case , How move?
    if(code > 0) return code;
    else return 0;
}

int get_tokencode_symbol(char _cbuf){
    char cbuf = _cbuf;
    int code;
    int strlen = 0;

    string_attr[strlen++] = cbuf;
    cbuf = read_char();
    // 2 symbol
    if(cbuf == '<'){
        if(cbuf == '>' || cbuf == '=') {
            string_attr[strlen++] = cbuf;
        }else{
            ungetc(cbuf,fp);
        }
    }else if(cbuf == '>' || cbuf == ':'){
        if(cbuf == '=') {
            string_attr[strlen++] = cbuf;
        }else{
            ungetc(cbuf,fp);
        }
    }else ungetc(cbuf,fp);
    code = check_strbuf(string_attr,strlen,0);
    //when code == TNAME, not symbol
    if(code == TNAME)return error("Invalid symbol input! (cant't use this symbol)");
    return code;
}

int check_string_format(char _cbuf){
    char cbuf = _cbuf;
    int strlen = 0;
    while(1){
        string_attr[strlen++] = cbuf;
        cbuf = read_char();
        if(cbuf == '\''){
            string_attr[strlen++] = cbuf;
            cbuf = read_char();
            if(cbuf == '\''){
                // continue string
            }else {
               ungetc(cbuf,fp);
               break;
            }
            //FIXME; EOF の場合
        }
    }
    return TSTRING;
}

// return
// -1 : not token
// mode
// 0:other
// 1:number
int check_strbuf(char *stringbuf,int strlen,int mode){
    stringbuf[strlen] = '\0';
    if(mode){
        num_attr = atoi(stringbuf);
        if(num_attr > 32768) return error("Integer is too large! Max is 32768.");
        else return TNUMBER;
    }
    int i = 0;
    for(i = 0;i < KEYWORDSIZE;i++){
        if(strcmp(key[i].keyword,stringbuf) == 0)return key[i].keytoken;
    }
    return TNAME;
}