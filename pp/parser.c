#include "parse.h"

int parse_program(){
    int token;
    token = scan();
    if(token != TPROGRAM) return error("ERROR: program must beign with \"program\"");
    token = scan();
    if(token != TNAME) return error("ERROR: required program name");
    token = scan();
    if(token != TSEMI) return error("ERROR: excpect \";\" next to program name");
    if(parse_block() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TDOT) return error("ERRPR: expect \".\" end of program");
    return 0;
}

int parse_block(){
    while(parse_var_dec() != S_ERROR || parse_sub_program() != S_ERROR){}
    if(parse_compound_statement() == S_ERROR) return S_ERROR;
    return 0;
}

int parse_var_dec(){
    int token;
    token = scan();
    if(token != TVAR) return S_ERROR;
    parse_variable_names();
    token = scan();
    if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
    parse_type();
    return 0;
}

int parse_variable_names(){
    int token;
    if(parse_variable_name() == S_ERROR) return S_ERROR;
    while((token = scan()) == TSEMI){
        if(parse_variable_name() == S_ERROR) return S_ERROR;
    }
    return 0;
}

int parse_variable_name(){
    int token = scan();
    if(token != TNAME) return error("ERROR: invarid variable declare");
    return 0;
}

int parse_type(){
    int token;
    if(parse_standard_type() == S_ERROR && parse_array_type() == S_ERROR)return error("ERROR:invarid type declare");
    return 0;
}

int parse_standard_type(){
    int token;
    token = scan();
    if(token != TINTEGER && token!= TBOOLEAN && token!= TCHAR) return S_ERROR;
    return 0;
}

int parse_array_type(){
    int token;
    token = scan();
    if(token != TARRAY) return S_ERROR;
    token = scan();
    if(token != TLSQPAREN) return error("ERROR: expect \"[\" next to \"array\"");
    token = scan();
    if(token != TNUMBER) return error("ERROR: invarid number input(array declare)");
    token = scan();
    if(token != TRSQPAREN) return error("ERROR: expect \"]\" next to number");
    token = scan();
    if(token != TOF) return error("ERROR: expect \"of\" next to \"]\"");
    if(parse_standard_type() == S_ERROR)return S_ERROR;
    return 0;
}