#include "scan.h"
#include <ctype.h>

char string_attr[MAXSTRSIZE];
int num_attr;

int scan(){
    char cbuf, prev;
    int strlen = 0;
    int code = 0;

    //When code is 0, skip this letter
    //FIXME: line num ope mut be added
    for(cbuf = read_char();code == 0;cbuf = read_char()){
        if(isspace(cbuf)){
            code = 0;
        }else if(isalpha(cbuf)){
            code = get_tokencode(0,cbuf);
        }else if(isdigit(cbuf)){
            code = get_tokencode(1,cbuf);
        }else if(cbuf == '\n'){
            code = 0;
        }else{
            switch(cbuf){
                //comment
                case '{':
                    for(cbuf = read_char();cbuf != '}';cbuf = read_char()){
                        if(cbuf == '\n') continue;
                        else if(cbuf == EOF || cbuf == '\0') return EOF;
                    };
                    code = 0;
                    break;
                case '/':
                    cbuf = read_char();
                    if(cbuf != '*') return error("Invalid symbol array!(Comment format)");
                    prev = cbuf; 
                    for(cbuf = read_char();prev != '*' || cbuf != '/';cbuf = read_char()){
                        prev = cbuf;
                        if(cbuf == '\n') continue;
                        else if(cbuf == EOF || cbuf == '\0') return EOF;
                    }
                    code = 0;
                    break;
                case '\'':
                    code = check_string_format(cbuf);
                    break;
                case EOF:
                    code = EOF;
                    break;
                default:
                    code = get_tokencode_symbol(cbuf);
                    //printf("%c %d",cbuf,code);
                    break;
            }
        }
    }
    if(cbuf == '\n'){
        // not put back buffer to stream
    }else ungetc(cbuf,fp);
    return code;
}

//mode
//  0: string or keyword
//  1: num
int get_tokencode(int mode, char _cbuf){
    char cbuf = _cbuf;
    int strlen = 0;
    int maxlen = mode ? 6 : MAXSTRSIZE-1;
    for(;isalpha(cbuf) || isdigit(cbuf);cbuf = read_char()){
        if(strlen >= maxlen) return error("Word length is too long!");
        string_attr[strlen++] = cbuf;
    }
    if(cbuf == '\n'){
        // not put back buffer to stream
    }
    //put back buffer to stream
    else ungetc(cbuf,fp);
    int code = check_strbuf(string_attr,strlen,mode);
    //FIXME: In case like 10case , How move?
    return code;
}

int get_tokencode_symbol(char _cbuf){
    char cbuf = _cbuf;
    int code;
    int strlen = 0;
    string_attr[strlen++] = cbuf;
    cbuf = read_char();
    // 2 symbol
    if(_cbuf == '<'){
        if(cbuf == '>' || cbuf == '=') {
            string_attr[strlen++] = cbuf;
        }else if(cbuf == '\n'){
        // not put back buffer to stream
        }else{
            ungetc(cbuf,fp);
        }
    }else if(_cbuf == '>' || _cbuf == ':'){
        if(cbuf == '=') {
            string_attr[strlen++] = cbuf;
        }else if(cbuf == '\n'){
        // not put back buffer to stream
        }else{
            ungetc(cbuf,fp);
        }
    }else if(cbuf == '\n'){
        // not put back buffer to stream
    }else ungetc(cbuf,fp);
    code = check_strbuf(string_attr,strlen,0);
    //printf("\n%d %s",code,string_attr);
    //when code == TNAME, not symbol
    if(code == TNAME)return error("Invalid symbol input! (cant't use this symbol)");
    return code;
}

int check_string_format(char _cbuf){
    char cbuf = _cbuf;
    int strlen = 0;
    while(1){
        string_attr[strlen++] = cbuf;
        if(strlen > MAXSTRSIZE-1)return error("ERROR: word is too long!");
        cbuf = read_char();

        if(cbuf == '\''){
            string_attr[strlen++] = cbuf;
            cbuf = read_char();
            if(cbuf == '\''){
                // continue string
            }else if(cbuf == '\n'){
                return error("Invalid string format!(Can't line break)");
            }else{
                ungetc(cbuf,fp);
                break;
            }
        }else if(cbuf == EOF){
            return EOF;
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
    for(i = 0;i < SYMBOLSIZE;i++){
        if(strcmp(sym[i].symbol,stringbuf) == 0)return sym[i].symtoken;
    }
    return TNAME;
}