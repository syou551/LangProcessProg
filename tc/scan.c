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
    for(cbuf = read_char();code == 0;cbuf = read_char()){
        if(isspace(cbuf)){
            code = 0;
        }else if(isalpha(cbuf)){
            code = get_tokencode(1,cbuf);
        }else if(isdigit(cbuf)){
            code = get_tokencode(0,cbuf);
        }else{
            switch(cbuf){
                //comment
                case '{':
                    for(cbuf = read_char();cbuf != '}';cbuf = read_char()){};
                    code = 0;
                    break;
                case '/': 
                    prev = cbuf; 
                    for(cbuf = read_char();prev != '*' && cbuf != '/';cbuf = read_char()){
                        prev = cbuf;
                    }
                    code = 0;
                    break;
                
            }
        }

    }
    return code;
}

//mode
//  1: string or keyword
//  0: num
int get_tokencode(int mode, char _cbuf){
    char cbuf = _cbuf;
    int strlen = 0;
    int maxlen = mode ? MAXSTRSIZE-1 : 6;
    if(mode == 0){
        digit_scan = 1;
    }
    for(;isalpha(cbuf);cbuf = read_char()){
        if(strlen >= maxlen) return error("Word length is too long!");
        string_attr[strlen++] = cbuf;
    }
    //put back buffer to stream
    ungetc(cbuf,fp);
    int code = check_strbuf(string_attr,strlen);
    //FIXME: In case like 10case , How move?
    if(code > 0) return code;
    else return 0;
}

// return
// -1 : not token
int check_strbuf(char *stringbuf,int strlen){
    stringbuf[strlen] = '\0';
    if(digit_scan){
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