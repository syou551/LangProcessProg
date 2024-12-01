#ifndef PARSE_H
#define PARSE_H

#include "scan.h"

//parser function
extern int parse_program();
extern int parse_block();
extern int parse_var_dec();
extern int parse_sub_program();
extern int parse_compound_statement();
extern int parse_statement();
extern int parse_variable_names();
extern int parse_variable_name();
extern int parse_type();
extern int parse_standard_type();
extern int parse_array_type();
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

//type check
extern int get_demand_type(int token);
extern int search_variable_type(char *np);
extern int search_variable_type_local(char *np);
extern int get_mode();
extern void set_mode_local();
extern void set_mode_global();

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