#ifndef IDLIST_H
#define IDLIST_H
//type check
#include <stdarg.h>
#define COLUMNNUM 4

struct TYPE
{
  int ttype;
  int arraysize;
  struct TYPE *etp;    // element type
  struct TYPE *paratp; // parameter type
};

struct LINE
{
  int linenum;
  struct LINE *nextlinep;
};

extern struct ID
{
  char *name;
  char *processname;
  struct TYPE *typ;
  int ispara;
  int deflinenum;
  struct LINE *reflinep;
  struct ID *nextp;
} *globalidroot, *localidroot;

extern int search_variable_type(char *np);
extern int search_variable_type_local(char *np);
extern int search_array_element_type(char *np);
int search_array_element_type_local(char *np);
extern int search_array_size(char *np);
int search_array_size_local(char *np);
extern struct TYPE *search_param_type(char *np);
struct TYPE *search_param_type_local(char *np);
extern int get_mode();
extern int set_mode_local(char *np);
extern void set_mode_global();
extern char *get_processname();
extern void set_param();
extern void reset_param();
extern int check_is_param(char *np);


extern void release_idtab(void);
void clear_idtab_local();
extern int id_add_variable(char *np, int deflinenum);
extern int id_add_info(char *np, int typetoken);
int id_add_info_local(char *np, char *processnp, int typetoken);
extern int id_add_element_info(char *np, int typetoken, int arraysize);
int id_add_element_info_local(char *np, int typetoken, int arraysize);
extern int id_add_param_info(int typetoken);
extern int id_add_reflinenum(char *np, int linenum);
int id_add_reflinenum_local(char *np, int linenum);
extern void print_idtab(void);

struct ID *sort_idtab(struct ID *p);
void check_column_size(struct ID *p);

#endif