#ifndef MAKECODE_H
#define MAKECODE_H

#include <stdio.h>
#include <stdarg.h>

extern FILE *cslfp;

extern int gen_new_label_num(void);
extern void gen_code(char *code,...);
extern void gen_code_label(char *code, int label);
extern void gen_label(int label);
extern char *get_symbol_keyword(int token);
extern void outlib(void);
extern void print_code(char *code,...);
extern void print_code_linebreak(void);


#endif