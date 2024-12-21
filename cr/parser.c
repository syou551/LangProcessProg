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
    token = scan();
    if(token != TEND) {
        print_linebreak();
        set_indent();
        print_indent();
    }else{
        print_linebreak();
        print_indent();
        print_symbol_keyword(token);

        return 0; 
    }
    if(parse_statement() == S_ERROR) return S_ERROR;

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
    int type;
    print_symbol_keyword(token);
    print_space();
    token = scan();
    if((type = parse_expression()) == S_ERROR) return S_ERROR;
    if(type != TBOOLEAN) return error("ERROR: expression in if statement must have boolean return value");
    
    if(token != TTHEN) return error("ERROR: expect \"then\" next to expression");
    print_space();
    print_symbol_keyword(token);
    print_linebreak();
    set_indent();
    print_indent();

    token = scan();
    if(parse_statement() == S_ERROR) return S_ERROR;

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

    int type;
    print_symbol_keyword(token);
    print_space();
    token = scan();
    if((type = parse_expression()) == S_ERROR) return S_ERROR;
    if(type != TBOOLEAN) return error("ERROR: expression in while statement must have boolean return value");

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
    if(exist_iteration < 1)
        return error("ERROR: break statement must be included  in at least one iteration statement");
    return 0;
}

int parse_call_statement(){
    int type;
    struct TYPE *t = NULL;
    print_symbol_keyword(token);
    token = scan();
    if(token != TNAME) return error("ERROR: expect procedure name");
    print_space();
    print_name_string(string_attr);
    if((type = search_variable_type(string_attr)) == S_ERROR) return S_ERROR;
    else if(type != TPROCEDURE) return error("ERROR: \"%s\" isn't procedure", string_attr);
    else if(get_mode() == LOCAL){
        if(strcmp(get_processname(),string_attr)==0) return error("ERROR: can't call procedure recursively");
    }
    
    if(id_add_reflinenum(string_attr,get_linenum()) == S_ERROR) return S_ERROR;
    t = search_param_type(string_attr);
    token = scan();
    if(token == TLPAREN){
        print_space();
        print_symbol_keyword(token);
        print_space();

        token = scan();
        if(parse_expressions(t) == S_ERROR) return S_ERROR;
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
    int type;
    print_symbol_keyword(token);
    token = scan();
    if(token == TLPAREN){
        print_space();
        print_symbol_keyword(token);
        print_space();

        token = scan();
        if((type = parse_variable()) == S_ERROR) return S_ERROR;
        if(type != TINTEGER && type != TCHAR) return error("ERROR: variable in input statement required integer or char type");
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
    //If string length is 0 or more than 1, the token is string
    if(token == TSTRING && strlen(string_attr) > 3 && strcmp(string_attr,"''''") != 0){
        //print
        print_space();
        print_name_string(string_attr);
        token = scan();
    }else{
        int type;
        print_space();
        if((type = parse_expression()) == S_ERROR) return S_ERROR;
        if(type != TINTEGER && type != TCHAR && type != TBOOLEAN) return error("ERROR: expression of output format required standard type");
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
    int type, exptype;
    if((type = parse_variable()) == S_ERROR)return S_ERROR;
    if(token != TASSIGN) return error("ERROR: expect \":=\" next to variable");
    print_space();
    print_symbol_keyword(token);
    print_space();

    token = scan();
    if((exptype = parse_expression())==S_ERROR)return S_ERROR;
    if(type != exptype) return error("ERROR: variable \"%s\" type don't match expression type", string_attr);
    return 0;
}

//add search variable type func 
int parse_variable(){
    int type, etype, asize, tmp;
    print_name_string(string_attr);
    if((type = search_variable_type(string_attr)) == S_ERROR) return S_ERROR;
    else if(type == TARRAY) {
        etype = search_array_element_type(string_attr);
        asize = search_array_size(string_attr);
    }

    if(id_add_reflinenum(string_attr,get_linenum()) == S_ERROR) return S_ERROR;

    token = scan();
    if(token == TLSQPAREN){
        if(type != TARRAY) return error("ERROR: this variable isn't array");
        print_space();
        print_symbol_keyword(token);

        print_space();
        token = scan();
        if((tmp = parse_expression()) == S_ERROR)return S_ERROR;
        if(tmp != TINTEGER) return error("ERROR: expression next to array type isn't integer");
        if(asize <= num_attr) return error("ERROR: out of bonds error.This array size is %d.", asize);
        if(token != TRSQPAREN)return error("ERROR: expect \"]\" next to expression");
        print_space();
        print_symbol_keyword(token);
        type = etype;
        token = scan();
    }
    return type;
}

int parse_expressions(struct TYPE *t){
    int type;
    struct TYPE *tp = t;
    if((type = parse_expression()) == S_ERROR) return S_ERROR;
    if(tp == NULL) return error("ERROR: this procedure don't have parameters");
    else if(tp->ttype != type) return error("ERROR: parametar type doesn't matched");
    tp = tp->paratp;
    while(token == TCOMMA){
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if((type = parse_expression()) == S_ERROR) return S_ERROR;
        if(tp == NULL) return error("ERROR: too many parameter");
        else if(tp->ttype != type) return error("ERROR: parametar type doesn't matched");
        tp = tp->paratp;
    }
    return 0;
}

int parse_expression(){
    int type, rtype, tmp;
    if((type = parse_simple_expression()) == S_ERROR) return S_ERROR;
    rtype = type;
    while(token == TEQUAL || token == TNOTEQ || token == TLE || token == TLEEQ || token == TGR || token == TGREQ){
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if((tmp = parse_simple_expression()) == S_ERROR) return S_ERROR;
        if(tmp != type) return error("ERROR: expression type don't match");
        rtype = TBOOLEAN;
    }
    return rtype;
}
//add return st.
//from here
int parse_simple_expression(){
    int type = 0 , termtype;
    if(token == TPLUS || token == TMINUS){
        print_symbol_keyword(token);
        print_space();
        type = TINTEGER;
        token = scan();
    }
    if((termtype = parse_term()) == S_ERROR) return S_ERROR;
    if(type == TINTEGER && termtype != type) {
        return error("ERROR: + or - is operator for integer");
    }

    type = termtype;
    while(token == TPLUS || token == TMINUS || token == TOR){
        if(get_demand_type(token) != type) return error("ERROR: operator is incorrect");
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if((termtype = parse_term()) == S_ERROR) return S_ERROR;
        if(termtype != type) return error("ERROR: term type don't match demand type");
    }
    return type;
}

int parse_term(){
    int type, tmp;
    if((type = parse_factor()) == S_ERROR) return S_ERROR;
    while(token == TSTAR || token == TDIV || token == TAND){
        if(get_demand_type(token) != type) return error("ERROR: operator incorrect");
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if((tmp = parse_factor()) == S_ERROR) return S_ERROR;
        if(tmp != type) return error("ERROR: incorrect factor type");
    }
    return type;
}

int parse_factor(){
    int type, tmp;
    if(token == TNAME){
        if((type = parse_variable()) == S_ERROR) return S_ERROR;
    }else if(token == TNUMBER || token == TFALSE || token == TTRUE || token == TSTRING){
        //print
        type = get_demand_type(token);
        if(token == TNUMBER || token == TSTRING) print_name_string(string_attr);
        else print_symbol_keyword(token);
        token = scan();
    }else if(token == TLPAREN){
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if((type = parse_expression()) == S_ERROR)return S_ERROR;
        if(token != TRPAREN) return error("ERROR: expect \")\" next to expression");
        print_space();
        print_symbol_keyword(token);
        token = scan();
    }else if(token == TNOT){
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if((type = parse_factor())== S_ERROR) return S_ERROR;
        if(type != TBOOLEAN) return error("ERROR: next \"not\" is required boolean");
    }else if(token == TINTEGER || token == TBOOLEAN || token == TCHAR){
        print_symbol_keyword(token);
        type = token;
        token = scan();
        if(token != TLPAREN) return error("ERROR: expect \"(\" next to standard type");
        print_space();
        print_symbol_keyword(token);
        
        print_space();
        token = scan();
        if((tmp = parse_expression()) == S_ERROR) return S_ERROR;
        if(tmp != TINTEGER && tmp != TBOOLEAN && tmp != TCHAR) return error("ERROR: cast is required standard type expression");
        if(token != TRPAREN) return error("ERROR: exprect \")\" next to expression");
        print_space();
        print_symbol_keyword(token);
        token = scan();
    }else return error("ERROR: expect factor");

    return type;
}
//#endregion

//#region variable

int set_variable_name(struct VNAME **root, int len){
    struct VNAME *newname = NULL;
    if((newname = (struct VNAME *)malloc(sizeof(struct VNAME))) == NULL) return error("ERROR: failure get space of variable");
    newname->name = NULL;
    newname->np = *root;
    if((newname->name = (char *)malloc(sizeof(len+1))) == NULL) return error("ERROR:failure get space of variable");
    strcpy(newname->name,string_attr);
    *root = newname;
    return 0;
}

void release_variable_name(struct VNAME **root){
    struct VNAME *p;
    if(*root == NULL) return;
    do{
        p = *root;
        *root = p->np;
        free(p->name);
        free(p);
    }while((*root) != NULL);
}

void add_variable_info(struct VNAME *root, int type){
    struct VNAME *p = NULL;
    p = root;
    while(p != NULL){
        id_add_info(p->name,type);
        p = p->np;
    }
}

void add_param_info(struct VNAME *root, int type){
    struct VNAME *p = NULL;
    p = root;
    while(p != NULL){
        id_add_info(p->name,type);
        if(get_mode() == LOCAL) id_add_param_info(type);
        p = p->np;
    }
}

void add_variable_info_array(struct VNAME *root, int type, struct ARRAYINFO *ainfo){
    struct VNAME *p = NULL;
    p = root;
    while(p != NULL){
        id_add_info(p->name,type);
        id_add_element_info(p->name,ainfo->etype,ainfo->size);
        p = p->np;
    }
}

int parse_var_dec(){
    struct VNAME *p = NULL;
    int type;
    if(parse_variable_names(&p) == S_ERROR) return S_ERROR;
    if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    //if array type, get array size and etype
    struct ARRAYINFO ainfo;
    if((type = parse_type(&ainfo)) == S_ERROR) return S_ERROR;

    if(type == TARRAY)add_variable_info_array(p,type,&ainfo);
    else add_variable_info(p,type);

    token = scan();
    if(token != TSEMI) return error("ERROR: expect \";\" next to type");
    print_symbol_keyword(token);
    print_linebreak(); 
    release_variable_name(&p);

    token = scan();
    while(token == TNAME){
        print_indent();
        if(parse_variable_names(&p) == S_ERROR) return S_ERROR;
        if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
        print_space();
        print_symbol_keyword(token);

        token = scan();
        if((type = parse_type(&ainfo)) == S_ERROR) return S_ERROR;
        
        if(type == TARRAY)add_variable_info_array(p,type,&ainfo);
        else add_variable_info(p,type);
        
        token = scan();
        if(token != TSEMI) return error("ERROR: expect \";\" next to type");
        print_symbol_keyword(token);
        print_linebreak();
        
        release_variable_name(&p); 
        token = scan();
    }
    return 0;
}

int parse_variable_names(struct VNAME **p){
    int len;
    struct VNAME *name_root = NULL;
    if((len = parse_variable_name()) == S_ERROR) return S_ERROR;
    if(set_variable_name(&name_root, len) == S_ERROR) return S_ERROR;
    token = scan();
    while(token == TCOMMA){
        print_space();
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if((len = parse_variable_name()) == S_ERROR) return S_ERROR;
        if(set_variable_name(&name_root, len) == S_ERROR) return S_ERROR;
        token = scan();
    }
    *p = name_root;
    return 0;
}

//return name length
int parse_variable_name(){
    if(token != TNAME) return error("ERROR: invarid variable name declare");
    print_name_string(string_attr);
    if(id_add_variable(string_attr, get_linenum()) == S_ERROR) return S_ERROR;
    return strlen(string_attr);
}

int parse_type(struct ARRAYINFO *ainfo){
    int type;
    if(token == TARRAY) {
        print_space();
        print_symbol_keyword(token);
        type = TARRAY;
        token = scan();
        if(parse_array_type(ainfo) == S_ERROR) return S_ERROR;
    }else if(token == TINTEGER || token == TBOOLEAN || token == TCHAR) {
        type = token;
        print_space();
        print_symbol_keyword(token);
    }else return error("ERROR: invarid type declare(expect integer, boolean, char or array)");
    return type;
}

int parse_standard_type(){
    if(token != TINTEGER && token!= TBOOLEAN && token!= TCHAR) return S_ERROR;
    print_space();
    print_symbol_keyword(token);

    return token;
}

int parse_array_type(struct ARRAYINFO *ainfo){
    int type;
    if(token != TLSQPAREN) return error("ERROR: expect \"[\" next to \"array\"");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    if(token != TNUMBER) return error("ERROR: invarid number input(array declare)");
    print_space();
    print_name_string(string_attr);
    ainfo->size = atoi(string_attr);
    if(ainfo->size < 1) error("ERROR: array size is required at least 1");

    token = scan();
    if(token != TRSQPAREN) return error("ERROR: expect \"]\" next to number");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    if(token != TOF) return error("ERROR: expect \"of\" next to \"]\"");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    if((type = parse_standard_type()) == S_ERROR)return S_ERROR;
    ainfo->etype = type;
    return 0;
}

//#endregion

//#region sub program
int parse_sub_program(){
    if(token != TNAME) return error("ERROR: expect procedure name");
    print_space();
    print_name_string(string_attr);
    if(id_add_variable(string_attr, get_linenum()) == S_ERROR) return S_ERROR;
    if(id_add_info(string_attr,TPROCEDURE) == S_ERROR) return S_ERROR;

    if(set_mode_local(string_attr) == S_ERROR) return S_ERROR;
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

    set_mode_global();
    return 0;
}

int parse_formal_parameters(){
    struct VNAME *p = NULL;
    int type;
    if(parse_variable_names(&p) == S_ERROR) return S_ERROR;
    if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
    print_space();
    print_symbol_keyword(token);

    token = scan();
    struct ARRAYINFO ainfo;
    if((type = parse_type(&ainfo)) == S_ERROR) return S_ERROR;

    //require only standard type
    if(type == TARRAY) return error("ERROR: formal parameters must be standard type");

    add_param_info(p,type);
    release_variable_name(&p);

    token = scan();
    while(token == TSEMI){
        print_symbol_keyword(token);
        print_space();
        token = scan();
        if(parse_variable_names(&p) == S_ERROR) return S_ERROR;
        if(token != TCOLON) return error("ERROR: expect \":\" next to variable names");
        print_space();
        print_symbol_keyword(token);

        token = scan();
        if((type = parse_type(&ainfo)) == S_ERROR) return S_ERROR;
        if(type == TARRAY) return error("ERROR: formal parameters must be standard type");
        
        add_variable_info(p,type);
        release_variable_name(&p);
        token = scan();
    }
    if(token != TRPAREN) return error("ERROR: expect \")\" next to variable names");
    print_space();
    print_symbol_keyword(token);

    return 0;
}

//#endregion

int get_demand_type(int token){
    int type = 0;
    switch(token){
        case TAND:
        case TOR: 
        case TTRUE:
        case TFALSE: type = TBOOLEAN; break;
        case TPLUS:
        case TMINUS:
        case TSTAR:
        case TDIV: 
        case TNUMBER: type = TINTEGER; break;
        case TSTRING: type = TCHAR; break;
    }
    return type;
}