#include "parse.h"

int indent = 0;

void print_symbol_keyword(int token){
    int i = 0;
    for(i = 0;i < KEYWORDSIZE;i++){
        if(token == key[i].keytoken){
            printf("%s",key[i].keyword);
            return;
        }
    }
    for(i = 0;i < SYMBOLSIZE; i++){
        if(token == sym[i].symtoken){
            printf("%s",sym[i].symbol);
            return;
        }
    }
    fflush(stdout);
}

void print_name_string(char* name){
    printf("%s",name);
    fflush(stdout);
}

void print_linebreak(){
    printf("\n");
    fflush(stdout);
}

void print_space(){
    printf(" ");
}

void set_indent(){
    indent++;
}
void remove_indent(){
    indent--;
}
void print_indent(){
    int i;
    for(int i = 0;i < indent;i++){
        printf("    ");
        fflush(stdout);
    }
}
