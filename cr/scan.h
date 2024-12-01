﻿#ifndef SCAN_H
#define SCAN_H

/* scan.h  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSTRSIZE 1024

/* Token */
#define	TNAME		1	/* Name : Alphabet { Alphabet | Digit } */
#define	TPROGRAM	2	/* program : Keyword */
#define	TVAR		3	/* var : Keyword */
#define	TARRAY		4	/* array : Keyword */
#define	TOF		5	/* of : Keyword */
#define	TBEGIN		6	/* begin : Keyword */
#define	TEND		7  	/* end : Keyword */
#define	TIF		8  	/* if : Keyword */
#define	TTHEN		9	/* then : Keyword */
#define	TELSE		10	/* else : Keyword */
#define	TPROCEDURE	11	/* procedure : Keyword */
#define	TRETURN		12	/* return : Keyword */
#define	TCALL		13	/* call : Keyword */
#define	TWHILE		14	/* while : Keyword */
#define	TDO		15 	/* do : Keyword */
#define	TNOT		16	/* not : Keyword */
#define	TOR		17	/* or : Keyword */
#define	TDIV		18 	/* div : Keyword */
#define	TAND		19 	/* and : Keyword */
#define	TCHAR		20	/* char : Keyword */
#define	TINTEGER	21	/* integer : Keyword */
#define	TBOOLEAN	22 	/* boolean : Keyword */
#define	TREADLN		23	/* readln : Keyword */
#define	TWRITELN	24	/* writeln : Keyword */
#define	TTRUE		25	/* true : Keyword */
#define	TFALSE		26	/* false : Keyword */
#define	TNUMBER		27	/* unsigned integer */
#define	TSTRING		28	/* String */
#define	TPLUS		29	/* + : symbol */
#define	TMINUS		30 	/* - : symbol */
#define	TSTAR		31 	/* * : symbol */
#define	TEQUAL		32 	/* = : symbol */
#define	TNOTEQ		33 	/* <> : symbol */
#define	TLE		34 	/* < : symbol */
#define	TLEEQ		35 	/* <= : symbol */
#define	TGR		36	/* > : symbol */
#define	TGREQ		37	/* >= : symbol */
#define	TLPAREN		38 	/* ( : symbol */
#define	TRPAREN		39 	/* ) : symbol */
#define	TLSQPAREN	40	/* [ : symbol */
#define	TRSQPAREN	41 	/* ] : symbol */
#define	TASSIGN		42	/* := : symbol */
#define	TDOT		43 	/* . : symbol */
#define	TCOMMA		44	/* , : symbol */
#define	TCOLON		45	/* : : symbol */
#define	TSEMI		46	/* ; : symbol */
#define	TREAD		47	/* read : Keyword */
#define	TWRITE		48	/* write : Keyword */
#define	TBREAK		49	/* break : Keyword */

#define NUMOFTOKEN	49

#define KEYWORDSIZE	28

#define SYMBOLSIZE 18

#define LOCAL 1
#define GLOBAL 0

#define S_ERROR  -1

extern struct KEY {
	char * keyword;
	int keytoken;
} key[KEYWORDSIZE];

// add variable declear
extern struct SYM {
	char * symbol;
	int symtoken;
} sym[SYMBOLSIZE];

extern int error(char *mes);

extern int init_scan(char *filename);
extern void init_idtab();
extern int scan(void);
extern int get_linenum(void);
extern void end_scan(void);
extern void release_idtab(void);
extern void id_add_variable(char *np);
extern void id_add_info(char *np, int typetoken, int deflinenum);
void id_add_info_local(char *np, char *processnp, int typetoken, int deflinenum);
extern void id_add_element_info(char *np, int typetoken, int arraysize);
extern void id_add_element_info_local(char *np, int typetoken, int arraysize);
extern void id_add_param_info(char *np, int typetoken);
extern void id_add_reflinenum(char *np, int linenum);
extern void id_add_reflinenum_local(char *np, int linenum);
extern void print_idtab(void);

// add function declear
extern void init_counter(int *counter_array);
extern void print_numtoken(int *counter_array, char *tokenstr[]);
extern char read_char(void);

int check_strbuf(char *stringbuf,int strlen,int mode);
int get_tokencode(int mode,char _cbuf);
int get_tokencode_symbol(char _cbuf);
int check_string_format(char _cbuf);

extern int num_attr;
extern char string_attr[MAXSTRSIZE];
extern FILE *fp;
extern int linenum;

#endif