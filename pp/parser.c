#include "parse.h"


int token;

int parse_program(){
    token = scan();
    if(token != TPROGRAM) return error("ERROR: program must beign with \"program\"");
    token = scan();
    if(token != TNAME) return error("ERROR: required program name");
    token = scan();
    if(token != TSEMI) return error("ERROR: excpect \";\" next to program name");
    token = scan();
    if(parse_block() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TDOT) return error("ERRPR: expect \".\" end of program");
    return 0;
}

int parse_block(){
    while(token == TVAR || token == TPROCEDURE){
        if(token == TVAR) {
            token = scan();
            if(parse_var_dec() == S_ERROR) return S_ERROR;
        }
        else if(token == TPROCEDURE) {
            token = scan();
            if(parse_sub_program() == S_ERROR) return S_ERROR;
            token = scan();
        }
    }
    if(parse_compound_statement() == S_ERROR) return S_ERROR;
    return 0;
}

//#region statement

int parse_compound_statement(){
    if(token != TBEGIN) return error("ERROR: expect \"begin\" next to \";\"");
    token = scan();
    if(parse_statement() == S_ERROR) return S_ERROR;
    token = scan();
    while(token == TSEMI){
        token = scan();
        if(parse_statement() == S_ERROR) return S_ERROR;
    }
    if(token != TEND) return error("ERROR: expect \"end\"");
    return 0;
}

int parse_statement(){
    if(token == TNAME) return parse_assign_statement();
    else if(token == TIF) return parse_if_statement();
    else if(token == TWHILE) return parse_while_statement();
    else if(token == TBREAK) return parse_break_statement();
    else if(token == TCALL) return parse_call_statement();
    else if(token == TRETURN) return parse_return_statement();
    else if(token == TBEGIN) return parse_compound_statement();
    else if(token == TREAD || token == TREADLN) return parse_input_statement();
    else if(token == TWRITE || token == TWRITELN) return parse_output_statement();
    //empty statement
    else return S_ERROR;
}

//#endregion

//#region variable
int parse_var_dec(){
    if(parse_variable_names() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
    token = scan();
    if(parse_type() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TSEMI) return error("ERROR: expect \";\" next to type");
    token = scan();
    while(token == TNAME){
        if(parse_variable_names()==S_ERROR) return S_ERROR;
        token = scan();
        if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
        token = scan();
        if(parse_type() == S_ERROR) return S_ERROR;
        token = scan();
        if(token != TSEMI) return error("ERROR: expect \";\" next to type");
        token = scan();
    }
    return 0;
}

int parse_variable_names(){
    if(parse_variable_name() == S_ERROR) return S_ERROR;
    token = scan();
    while(token == TSEMI){
        token = scan();
        if(parse_variable_name() == S_ERROR) return S_ERROR;
        token = scan();
    }
    return 0;
}

int parse_variable_name(){
    if(token != TNAME) return error("ERROR: invarid variable name declare");
    return 0;
}

int parse_type(){
    if(token == TINTEGER || token == TBOOLEAN || token == TCHAR || token == TARRAY){ 
        if(token == TARRAY) {
            token = scan();
            if(parse_array_type() == S_ERROR) return S_ERROR;
        }else if(token == TINTEGER || token == TBOOLEAN || token == TCHAR) {
            token = scan();
            if(parse_standard_type() == S_ERROR) return S_ERROR;
        }
    }else error("ERROR: invarid type declare(expect integer, boolean, char or array)");
    return 0;
}

int parse_standard_type(){
    if(token != TINTEGER && token!= TBOOLEAN && token!= TCHAR) return S_ERROR;
    return 0;
}

int parse_array_type(){
    if(token != TLSQPAREN) return error("ERROR: expect \"[\" next to \"array\"");
    token = scan();
    if(token != TNUMBER) return error("ERROR: invarid number input(array declare)");
    token = scan();
    if(token != TRSQPAREN) return error("ERROR: expect \"]\" next to number");
    token = scan();
    if(token != TOF) return error("ERROR: expect \"of\" next to \"]\"");
    token = scan();
    if(parse_standard_type() == S_ERROR)return S_ERROR;
    return 0;
}

//#endregion

//#region sub program
int parse_sub_program(){
    if(token != TNAME) return error("ERROR: expect procedure name");
    token = scan();
    if(token == TLPAREN) {
        token = scan();
        if(parse_formal_parameters() == S_ERROR) return S_ERROR;
    }
    token = scan();
    if(token != TSEMI) return error("ERROR: expect \";\" next to procedure name");
    token = scan();
    if(token == TVAR) {
        token = scan();
        if(parse_var_dec() == S_ERROR) return S_ERROR;
    }
    token = scan();
    if(parse_compound_statement() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TSEMI) return error("ERROR: expect \";\" next to compound statement");
    return 0;
}

int parse_formal_parameters(){
    if(parse_variable_names() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
    token = scan();
    if(parse_type() == S_ERROR) return S_ERROR;
    token = scan();
    while(token == TSEMI){
        token = scan();
        if(parse_variable_names() == S_ERROR) return S_ERROR;
        token = scan();
        if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
        token = scan();
        if(parse_type() == S_ERROR) return S_ERROR;
        token = scan();
    }
    if(token != TRPAREN) return error("ERROR: expect \")\" next to variable names");
    return 0;
}

//#endregion