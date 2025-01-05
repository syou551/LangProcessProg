#ifndef PARSE_H
#define PARSE_H

#define DEBUG 

#include "scan.h"
#include "idlist.h"
#include "makeCode.h"

//remember variable names declared same state
struct VNAME{
    char *name;
    struct VNAME *np;
};
//array type size and element type
struct ARRAYINFO{
    int size;
    int etype;
};

extern char *tokenstr[NUMOFTOKEN + 1];

//parser function
extern int parse_program();
extern int parse_block();
extern int parse_var_dec();
extern int parse_sub_program();
extern int parse_compound_statement();
extern int parse_statement();
extern int parse_variable_names(struct VNAME **p);
extern int parse_variable_name();
extern int parse_type(struct ARRAYINFO *ainfo);
extern int parse_standard_type();
extern int parse_array_type(struct ARRAYINFO *ainfo);
extern int parse_formal_parameters();

extern int parse_assign_statement();
extern int parse_if_statement();
extern int parse_while_statement();
extern int parse_break_statement();
extern int parse_call_statement();
extern int parse_return_statement();
extern int parse_input_statement(int st_token);
extern int parse_output_statement(int st_token);

extern int parse_variable();
extern int parse_expression();
extern int parse_expressions(struct TYPE *t);
extern int parse_simple_expression();
extern int parse_term();
extern int parse_factor();
extern int parse_constant();

extern int parse_output_format();

extern int get_demand_type(int token);

//prety printer

#ifdef DEBUG
extern void print_symbol_keyword(int token);
extern void print_name_string(char* name);
extern void print_linebreak();
extern void print_space();
extern void print_indent();
extern void set_indent();
extern void remove_indent();

#else
#define print_symbol_keyword(token) token = token
#define print_name_string(string_attr) token = token
#define print_linebreak() token = token
#define print_space() token = token
#define print_indent() token = token
#define set_indent() token = token
#define remove_indent() token = token
#endif

extern int indent;

#endif