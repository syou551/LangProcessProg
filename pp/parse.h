#ifndef PARSE_H
#define PARSE_H

#include "scan.h"

extern int parse_program();
extern int parse_block();
extern int parse_var_dec();
extern int parse_sub_program();
extern int parse_compound_statement();
extern int parse_variable_names();
extern int parse_variable_name();
extern int parse_type();
extern int parse_standard_type();
extern int parse_array_type();

#endif