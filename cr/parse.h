#ifndef PARSE_H
#define PARSE_H

#include "scan.h"

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
extern int parse_input_statement();
extern int parse_output_statement();

extern int parse_variable();
extern int parse_expression();
extern int parse_expressions();
extern int parse_simple_expression();
extern int parse_term();
extern int parse_factor();
extern int parse_constant();

extern int parse_output_format();

extern int get_demand_type(int token);

//prety printer
extern void print_symbol_keyword(int token);
extern void print_name_string(char* name);
extern void print_linebreak();
extern void print_space();
extern void print_indent();
extern void set_indent();
extern void remove_indent();

extern int indent;

#endif