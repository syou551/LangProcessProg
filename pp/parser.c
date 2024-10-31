#include "parse.h"


int token;
int exist_iteration;

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
    //token = scan(); // Is this statement required? assignment: No if:No while:No
    while(token == TSEMI){
        token = scan();
        if(parse_statement() == S_ERROR) return S_ERROR;
    }
    if(token != TEND) return error("ERROR: expect \"end\"");
    return 0;
}

// when this func has ended, what value is in token?
int parse_statement(){
    if(token == TNAME) return parse_assign_statement();
    else if(token == TIF) return parse_if_statement();
    else if(token == TWHILE) return parse_while_statement();
    else if(token == TBREAK) return parse_break_statement();
    else if(token == TCALL) return parse_call_statement();
    else if(token == TRETURN) return parse_return_statement();
    else if(token == TBEGIN){
        if(parse_compound_statement() == S_ERROR) return S_ERROR;
        token = scan();
        return 0;
    }
    else if(token == TREAD || token == TREADLN) return parse_input_statement();
    else if(token == TWRITE || token == TWRITELN) return parse_output_statement();
    //empty statement
    else return 0;
}

int parse_if_statement(){
    token = scan();
    if(parse_expression() == S_ERROR) return S_ERROR;
    if(token != TTHEN) return error("ERROR: expect \"then\" next to expression");
    token = scan();
    if(parse_statement() == S_ERROR) return S_ERROR;
    //token = scan(); // Is this statement required? assignment: No
    if(token == TELSE){
        token = scan();
        if(parse_statement() == S_ERROR) return S_ERROR;
    }
    return 0;
}

int parse_while_statement(){
    //no less than 1 break statement, so count number of appearing it
    exist_iteration++;
    token = scan();
    if(parse_expression() == S_ERROR) return S_ERROR;
    if(token != TDO) return error("ERROR: exprect \"do\" next to expression");
    token = scan();
    if(parse_statement() == S_ERROR) return S_ERROR;
    exist_iteration--;
    return 0;
}

int parse_break_statement(){
    //print
    token = scan();
    if(exist_iteration < 1){
        return error("ERROR: break statement must be included  in at least one iteration statement");
    }
    return 0;
}

int parse_call_statement(){
    token = scan();
    if(token != TNAME) return error("ERROR: expect procedure name");
    token = scan();
    if(token == TLPAREN){
        token = scan();
        if(parse_expressions() == S_ERROR) return S_ERROR;
        if(token != TRPAREN) return error("ERROR: expect \")\" next to expressions");
        token = scan();
    }
    return 0;
}

int parse_return_statement(){
    //print
    token = scan();
    return 0;
}

int parse_input_statement(){
    token = scan();
    if(token == TLPAREN){
        token = scan();
        if(parse_variable() == S_ERROR) return S_ERROR;
        while(token == TCOMMA){
            token = scan();
            if(parse_variable() == S_ERROR) return S_ERROR;
        }
        if(token != TRPAREN) return error("ERROR: expect \")\" next to variable");
        token = scan();
    }
    return 0;
}

int parse_output_statement(){
    token = scan();
    if(token == TLPAREN){
        token = scan();
        if(parse_output_format() == S_ERROR) return S_ERROR;
        while(token == TCOMMA){
            token = scan();
            if(parse_output_format() == S_ERROR) return S_ERROR;
        }
        if(token != TRPAREN) return error("ERROR: expect \")\" next to output format");
        token = scan();
    }
    return 0; 
}

int parse_output_format(){
    //If string length is more than 1, the token is string
    if(token == TSTRING && strlen(string_attr) > 1 && strcmp(string_attr,"''''") != 0){
        //print
        token = scan();
    }else{
        if(parse_expression() == S_ERROR) return S_ERROR;
        if(token == TCOLON){
            token = scan();
            if(token != TNUMBER) return error("ERROR: exprect number next to \":\"");
            token = scan();
        }
    }
    return 0;
}

int parse_assign_statement(){
    //lepf part is equal to variable
    if(parse_variable() == S_ERROR)return S_ERROR;
    if(token != TASSIGN) return error("ERROR: expect \":=\" next to variable");
    token = scan();
    if(parse_expression()==S_ERROR)return S_ERROR;
    return 0;
}

int parse_variable(){
    //if(parse_variable_name() == S_ERROR)return S_ERROR; jimei
    token = scan();
    if(token == TLSQPAREN){
        token = scan();
        if(parse_expression() == S_ERROR)return S_ERROR;
        if(token != TRSQPAREN)return error("ERROR: expect \"]\" next to expression");
        token = scan();
    }
    return 0;
}

int parse_expressions(){
    if(parse_expression() == S_ERROR) return S_ERROR;
    while(token == TCOMMA){
        token = scan();
        if(parse_expression() == S_ERROR) return S_ERROR;
    }
    return 0;
}

int parse_expression(){
    if(parse_simple_expression() == S_ERROR) return S_ERROR;
    while(token == TEQUAL || token == TNOTEQ || token == TLE || token == TLEEQ || token == TGR || token == TGREQ){
        
        token = scan();
        if(parse_simple_expression() == S_ERROR) return S_ERROR;
    }
    return 0;
}

int parse_simple_expression(){
    if(token == TPLUS || token == TMINUS){
        token = scan();
    }
    if(parse_term() == S_ERROR) return S_ERROR;
    while(token == TPLUS || token == TMINUS || token == TOR){
        token = scan();
        if(parse_term() == S_ERROR) return S_ERROR;
    }
    return 0;
}

int parse_term(){
    if(parse_factor() == S_ERROR) return S_ERROR;
    while(token == TSTAR || token == TDIV || token == TAND){
        token = scan();
        if(parse_factor() == S_ERROR) return S_ERROR;
    }
    return 0;
}

int parse_factor(){
    if(token == TNAME){
        if(parse_variable() == S_ERROR) return S_ERROR;
    }else if(token == TNUMBER || token == TFALSE || token == TTRUE || token == TSTRING){
        //print
        token = scan();
    }else if(token == TLPAREN){
        token = scan();
        if(parse_expression() == S_ERROR)return S_ERROR;
        if(token != TRPAREN) return error("ERROR: expect \")\" next to expression");
        token = scan();
    }else if(token == TNOT){
        token = scan();
        if(parse_factor() == S_ERROR) return S_ERROR;
    }else if(token == TINTEGER || token == TBOOLEAN || token == TCHAR){
        token = scan();
        if(token != TLPAREN) return error("ERROR: expect \"(\" next to standard type");
        token = scan();
        if(parse_expression() == S_ERROR) return S_ERROR;
        if(token != TRPAREN) return error("ERROR: exprect \")\" next to expression");
        token = scan();
    }else return error("ERROR: expect factor");
    return 0;
}
//#endregion

//#region variable
int parse_var_dec(){
    if(parse_variable_names() == S_ERROR) return S_ERROR;
    if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
    token = scan();
    if(parse_type() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TSEMI) return error("ERROR: expect \";\" next to type");
    token = scan();
    while(token == TNAME){
        if(parse_variable_names()==S_ERROR) return S_ERROR;
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
    while(token == TCOMMA){
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
    if(token == TARRAY) {
        token = scan();
        if(parse_array_type() == S_ERROR) return S_ERROR;
    }else if(token == TINTEGER || token == TBOOLEAN || token == TCHAR) {
        //if(parse_standard_type() == S_ERROR) return S_ERROR; can removed statement
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
        token = scan();
    }
    if(token != TSEMI) return error("ERROR: expect \";\" next to procedure name");
    token = scan();
    if(token == TVAR) {
        token = scan();
        if(parse_var_dec() == S_ERROR) return S_ERROR;
    }
    if(parse_compound_statement() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TSEMI) return error("ERROR: expect \";\" next to compound statement");
    return 0;
}

int parse_formal_parameters(){
    if(parse_variable_names() == S_ERROR) return S_ERROR;
    if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
    token = scan();
    if(parse_type() == S_ERROR) return S_ERROR;
    token = scan();
    while(token == TSEMI){
        token = scan();
        if(parse_variable_names() == S_ERROR) return S_ERROR;
        if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
        token = scan();
        if(parse_type() == S_ERROR) return S_ERROR;
        token = scan();
    }
    if(token != TRPAREN) return error("ERROR: expect \")\" next to variable names");
    return 0;
}

//#endregion