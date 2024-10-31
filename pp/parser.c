#include "parse.h"

int token;
int exist_iteration;

int parse_program(){
    token = scan();
    if(token != TPROGRAM) return error("ERROR: program must beign with \"program\"");
    print_symbol_keyword(token);
    print_space();
    
    token = scan();
    if(token != TNAME) return error("ERROR: required program name");
    print_name_string(string_attr);

    token = scan();
    if(token != TSEMI) return error("ERROR: excpect \";\" next to program name");
    print_symbol_keyword(token);
    print_linebreak();
    set_indent();

    token = scan();
    if(parse_block() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TDOT) return error("ERRPR: expect \".\" end of program");
    print_symbol_keyword(token);
    print_linebreak();
    return 0;
}

int parse_block(){
    while(token == TVAR || token == TPROCEDURE){
        print_indent();
        print_symbol_keyword(token);
        if(token == TVAR) {
            print_linebreak();
            set_indent();
            print_indent();
            token = scan();
            if(parse_var_dec() == S_ERROR) return S_ERROR;
            remove_indent();
        }
        else if(token == TPROCEDURE) {
            token = scan();
            if(parse_sub_program() == S_ERROR) return S_ERROR;
            token = scan();
        }
    }
    remove_indent();
    print_indent();
    if(parse_compound_statement() == S_ERROR) return S_ERROR;
    return 0;
}

//#region statement

int parse_compound_statement(){
    if(token != TBEGIN) return error("ERROR: expect \"begin\" next to \";\"");
    print_symbol_keyword(token);
    print_linebreak();
    set_indent();
    print_indent();
    token = scan();
    if(parse_statement() == S_ERROR) return S_ERROR;
    //token = scan(); // Is this statement required? assignment: No if:No while:No
    while(token == TSEMI){
        print_symbol_keyword(token);
        token = scan();
        if(token == TEND) break;
        print_linebreak();
        print_indent();
        if(parse_statement() == S_ERROR) return S_ERROR;
    }
    remove_indent();
    print_linebreak();

    if(token != TEND) return error("ERROR: expect \"end\"");
    print_indent();
    print_symbol_keyword(token);

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
    print_symbol_keyword(token);
    print_space();
    token = scan();
    if(parse_expression() == S_ERROR) return S_ERROR;
    if(token != TTHEN) return error("ERROR: expect \"then\" next to expression");
    print_space();
    print_symbol_keyword(token);
    print_linebreak();
    set_indent();
    print_indent();

    token = scan();
    if(parse_statement() == S_ERROR) return S_ERROR;
    //token = scan(); // Is this statement required? assignment: No
    remove_indent();
    if(token == TELSE){
        print_linebreak();
        print_indent();
        print_symbol_keyword(token);
        print_linebreak();
        set_indent();
        print_indent();
        
        token = scan();
        if(parse_statement() == S_ERROR) return S_ERROR;
        remove_indent();
    }
    return 0;
}

int parse_while_statement(){
    //no less than 1 break statement, so count number of appearing it
    exist_iteration++;
    print_symbol_keyword(token);
    print_space();
    token = scan();
    if(parse_expression() == S_ERROR) return S_ERROR;
    if(token != TDO) return error("ERROR: exprect \"do\" next to expression");
    print_space();
    print_symbol_keyword(token);
    print_linebreak();
    set_indent();
    print_indent();

    token = scan();
    if(parse_statement() == S_ERROR) return S_ERROR;
    exist_iteration--;
    remove_indent();
    return 0;
}

int parse_break_statement(){
    print_symbol_keyword(token);
    //print
    token = scan();
    if(exist_iteration < 1){
        return error("ERROR: break statement must be included  in at least one iteration statement");
    }
    return 0;
}

int parse_call_statement(){
    print_symbol_keyword(token);
    token = scan();
    if(token != TNAME) return error("ERROR: expect procedure name");
    print_space();
    print_name_string(string_attr);

    token = scan();
    if(token == TLPAREN){
        print_space();
        print_symbol_keyword(token);
        print_space();

        token = scan();
        if(parse_expressions() == S_ERROR) return S_ERROR;
        if(token != TRPAREN) return error("ERROR: expect \")\" next to expressions");
        print_space();
        print_symbol_keyword(token);
        token = scan();
    }
    return 0;
}

int parse_return_statement(){
    //print
    print_symbol_keyword(token);
    token = scan();
    return 0;
}

int parse_input_statement(){
    print_symbol_keyword(token);
    token = scan();
    if(token == TLPAREN){
        print_space();
        print_symbol_keyword(token);
        print_space();

        token = scan();
        if(parse_variable() == S_ERROR) return S_ERROR;
        while(token == TCOMMA){
            print_space();
            print_symbol_keyword(token);
            print_space();
            token = scan();
            if(parse_variable() == S_ERROR) return S_ERROR;
        }
        if(token != TRPAREN) return error("ERROR: expect \")\" next to variable");
        print_space();
        print_symbol_keyword(token);
        token = scan();
    }
    return 0;
}

int parse_output_statement(){
    print_symbol_keyword(token);
    token = scan();
    if(token == TLPAREN){
        print_space();
        print_symbol_keyword(token);
        token = scan();
        if(parse_output_format() == S_ERROR) return S_ERROR;
        while(token == TCOMMA){
            print_space();
            print_symbol_keyword(token);
            token = scan();
            if(parse_output_format() == S_ERROR) return S_ERROR;
        }
        if(token != TRPAREN) return error("ERROR: expect \")\" next to output format");
        print_space();
        print_symbol_keyword(token);
        token = scan();
    }
    return 0; 
}

int parse_output_format(){
    //If string length is more than 1, the token is string
    if(token == TSTRING && strlen(string_attr) > 1 && strcmp(string_attr,"''''") != 0){
        //print
        print_space();
        print_name_string(string_attr);
        token = scan();
    }else{
        print_space();
        if(parse_expression() == S_ERROR) return S_ERROR;
        if(token == TCOLON){
            print_space();
            print_symbol_keyword(token);
            token = scan();
            if(token != TNUMBER) return error("ERROR: exprect number next to \":\"");
            print_space();
            print_name_string(string_attr);
            token = scan();
        }
    }
    return 0;
}

int parse_assign_statement(){
    //lepf part is equal to variable
    if(parse_variable() == S_ERROR)return S_ERROR;
    if(token != TASSIGN) return error("ERROR: expect \":=\" next to variable");
    print_space();
    print_symbol_keyword(token);
    print_space();

    token = scan();
    if(parse_expression()==S_ERROR)return S_ERROR;
    return 0;
}

int parse_variable(){
    //if(parse_variable_name() == S_ERROR)return S_ERROR; jimei
    print_name_string(string_attr);
    token = scan();
    if(token == TLSQPAREN){
        print_space();
        print_symbol_keyword(token);

        print_space();
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
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if(parse_expression() == S_ERROR) return S_ERROR;
    }
    return 0;
}

int parse_expression(){
    if(parse_simple_expression() == S_ERROR) return S_ERROR;
    while(token == TEQUAL || token == TNOTEQ || token == TLE || token == TLEEQ || token == TGR || token == TGREQ){
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if(parse_simple_expression() == S_ERROR) return S_ERROR;
    }
    return 0;
}

int parse_simple_expression(){
    if(token == TPLUS || token == TMINUS){
        print_symbol_keyword(token);
        print_space();
        token = scan();
    }
    if(parse_term() == S_ERROR) return S_ERROR;
    while(token == TPLUS || token == TMINUS || token == TOR){
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if(parse_term() == S_ERROR) return S_ERROR;
    }
    return 0;
}

int parse_term(){
    if(parse_factor() == S_ERROR) return S_ERROR;
    while(token == TSTAR || token == TDIV || token == TAND){
        print_space();
        print_symbol_keyword(token);
        print_space();
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
        if(token == TNUMBER || token == TSTRING) print_name_string(string_attr);
        else print_symbol_keyword(token);
        token = scan();
    }else if(token == TLPAREN){
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if(parse_expression() == S_ERROR)return S_ERROR;
        if(token != TRPAREN) return error("ERROR: expect \")\" next to expression");
        print_space();
        print_symbol_keyword(token);
        token = scan();
    }else if(token == TNOT){
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if(parse_factor() == S_ERROR) return S_ERROR;
    }else if(token == TINTEGER || token == TBOOLEAN || token == TCHAR){
        print_symbol_keyword(token);
        token = scan();
        if(token != TLPAREN) return error("ERROR: expect \"(\" next to standard type");
        print_space();
        print_symbol_keyword(token);
        
        print_space();
        token = scan();
        if(parse_expression() == S_ERROR) return S_ERROR;
        if(token != TRPAREN) return error("ERROR: exprect \")\" next to expression");
        print_space();
        print_symbol_keyword(token);
        token = scan();
    }else return error("ERROR: expect factor");
    return 0;
}
//#endregion

//#region variable
int parse_variable_names_and_type(){
    if(parse_variable_names() == S_ERROR) return S_ERROR;
    if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    if(parse_type() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TSEMI) return error("ERROR: expect \";\" next to type");
    print_symbol_keyword(token);
    print_linebreak(); 
    return 0;
}

int parse_var_dec(){
    if(parse_variable_names_and_type() == S_ERROR) return S_ERROR;

    token = scan();
    while(token == TNAME){
        print_indent();
        if(parse_variable_names_and_type() == S_ERROR) return S_ERROR;
        token = scan();
    }
    return 0;
}

int parse_variable_names(){
    if(parse_variable_name() == S_ERROR) return S_ERROR;
    token = scan();
    while(token == TCOMMA){
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if(parse_variable_name() == S_ERROR) return S_ERROR;
        token = scan();
    }
    return 0;
}

int parse_variable_name(){
    if(token != TNAME) return error("ERROR: invarid variable name declare");
    print_name_string(string_attr);
    return 0;
}

int parse_type(){
    if(token == TARRAY) {
        print_space();
        print_symbol_keyword(token);

        token = scan();
        if(parse_array_type() == S_ERROR) return S_ERROR;
    }else if(token == TINTEGER || token == TBOOLEAN || token == TCHAR) {
        print_space();
        print_symbol_keyword(token);
    }else error("ERROR: invarid type declare(expect integer, boolean, char or array)");
    return 0;
}

int parse_standard_type(){
    if(token != TINTEGER && token!= TBOOLEAN && token!= TCHAR) return S_ERROR;
    print_space();
    print_symbol_keyword(token);

    return 0;
}

int parse_array_type(){
    if(token != TLSQPAREN) return error("ERROR: expect \"[\" next to \"array\"");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    if(token != TNUMBER) return error("ERROR: invarid number input(array declare)");
    print_space();
    print_name_string(string_attr);

    token = scan();
    if(token != TRSQPAREN) return error("ERROR: expect \"]\" next to number");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    if(token != TOF) return error("ERROR: expect \"of\" next to \"]\"");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    if(parse_standard_type() == S_ERROR)return S_ERROR;
    return 0;
}

//#endregion

//#region sub program
int parse_sub_program(){
    if(token != TNAME) return error("ERROR: expect procedure name");
    print_space();
    print_name_string(string_attr);

    token = scan();
    if(token == TLPAREN) {
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if(parse_formal_parameters() == S_ERROR) return S_ERROR;
        token = scan();
    }
    if(token != TSEMI) return error("ERROR: expect \";\" next to procedure name");
    print_symbol_keyword(token);
    print_linebreak();

    token = scan();
    if(token == TVAR) {
        set_indent();
        print_indent();
        print_symbol_keyword(token);
        print_linebreak();
        set_indent();
        print_indent();

        token = scan();
        if(parse_var_dec() == S_ERROR) return S_ERROR;
        remove_indent();
        remove_indent();
    }
    print_indent();
    if(parse_compound_statement() == S_ERROR) return S_ERROR;
    token = scan();
    if(token != TSEMI) return error("ERROR: expect \";\" next to compound statement");
    print_symbol_keyword(token);
    print_linebreak();
    return 0;
}

int parse_formal_parameters(){
    if(parse_variable_names() == S_ERROR) return S_ERROR;
    if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    if(parse_type() == S_ERROR) return S_ERROR;
    token = scan();
    while(token == TSEMI){
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if(parse_variable_names() == S_ERROR) return S_ERROR;
        if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
        print_space();
        print_symbol_keyword(token);

        token = scan();
        if(parse_type() == S_ERROR) return S_ERROR;
        token = scan();
    }
    if(token != TRPAREN) return error("ERROR: expect \")\" next to variable names");
    print_space();
    print_symbol_keyword(token);

    return 0;
}

//#endregion