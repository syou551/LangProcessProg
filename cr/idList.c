#include "parse.h"
#include "idlist.h"

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

struct ID
{
  char *name;
  char *processname;
  struct TYPE *typ;
  int ispara;
  int deflinenum;
  struct LINE *reflinep;
  struct ID *nextp;
} *globalidroot, *localidroot;

static int mode = GLOBAL;
static char *processname = NULL;

void init_idtab()
{ /* Initialise the table */
  globalidroot = NULL;
  localidroot = NULL;
}

struct ID *search_idtab(char *np)
{ /* search the name pointed by np */
  struct ID *p;

  for (p = globalidroot; p != NULL; p = p->nextp)
  {
    if (!strcmp(np, p->name)){
      if(!p->ispara)
        return (p);
    }
  }
  return (NULL);
}

struct ID *search_idtab_local(char *np)
{
  struct ID *p;
  for (p = localidroot; p != NULL; p = p->nextp)
  {
    if (!strcmp(np, p->name))
      return (p);
  }
  return (NULL);
}

void id_add_variable(char *np){
  struct ID *p;
  struct TYPE *t;
  struct LINE *l;
  char *cp;
  if ((p = (struct ID *)malloc(sizeof(struct ID))) == NULL)
  {
    error("Cannot malloc for p in id_add_info");
    return;
  }
  if ((cp = (char *)malloc(strlen(np) + 1)) == NULL)
  {
    error("Cannot malloc for cp in id_add_info");
    return;
  }
  if ((t = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
  {
    error("Cannot malloc for t in id_add_info");
    return;
  }
  if ((l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)
  {
    error("Cannot malloc for l in id_add_info");
    return;
  }
  strcpy(cp, np);
  p->name = cp;
  p->processname = NULL;
  p->ispara = get_mode();

  p->typ = t;
  p->deflinenum = S_ERROR;
  if(get_mode() == GLOBAL){
    p->nextp = globalidroot;
    globalidroot = p;
  }else if(get_mode() == LOCAL){
    p->nextp = localidroot;
    localidroot = p;
  }
  t->ttype = S_ERROR;
  t->arraysize = 0;
  t->etp = NULL;
  t->paratp = NULL;
}

void id_add_info(char *np, int typetoken, int deflinenum)
{
  struct ID *p = NULL;
  switch(mode){
    case 0 : p = search_idtab(np);break;
    case 1 : id_add_info_local(np, processname, typetoken, deflinenum);return;
  }
  if(p == NULL) {
    error("ERROR: internal error(variable isn't added table)");
    return;
  }
  struct TYPE *t = p->typ;
  struct LINE *l = p->reflinep;
  char *cp;

  p->deflinenum = deflinenum;
  t->ttype = typetoken;
  t->arraysize = 0;
  t->etp = NULL;
  t->paratp = NULL;
}

void id_add_info_local(char *np, char *processnp, int typetoken, int deflinenum)
{
  struct ID *p = NULL;
  if((p = search_idtab(np)) == NULL) {
    error("ERROR: internal error(variable isn't added table)");
    return;
  }
  struct TYPE *t = p->typ;
  struct LINE *l = p->reflinep;
  char *cp, *procp;
  if ((procp = (char *)malloc(strlen(processnp) + 1)) == NULL)
  {
    error("Cannot malloc for procp in id_add_info_local");
    return;
  }
  strcpy(procp, processnp);
  p->processname = procp;
  p->deflinenum = deflinenum;
  t->ttype = typetoken;
  t->arraysize = 0;
  t->etp = NULL;
  t->paratp = NULL;
}

void id_add_element_info(char *np, int typetoken, int arraysize)
{
  struct ID *p;
  struct TYPE *t;
  switch(mode){
    case 0 : p = search_idtab(np);break;
    case 1 : id_add_element_info_local(np, typetoken, arraysize);return;
  }
  if (p != NULL)
  {
    if ((t = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
    {
      error("Cannot malloc for t in id_add_element_info");
      return;
    }
    p->typ->etp = t;
    t->ttype = typetoken;
    t->arraysize = arraysize;
    t->etp = NULL;
    t->paratp = NULL;
  }
  else
  {
    error("Cannnot find array id data!");
    return;
  }
}

void id_add_element_info_local(char *np, int typetoken, int arraysize)
{
  struct ID *p;
  struct TYPE *t;
  if ((p = search_idtab_local(np)) != NULL)
  {
    if ((t = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
    {
      error("Cannot malloc for t in id_add_element_info");
      return;
    }
    p->typ->etp = t;
    t->ttype = typetoken;
    t->arraysize = arraysize;
    t->etp = NULL;
    t->paratp = NULL;
  }
  else
  {
    error("Cannnot find array id data!");
    return;
  }
}

void id_add_param_info(char *np, int typetoken)
{
  struct ID *p;
  struct TYPE *t, *end;
  if ((p = search_idtab(np)) != NULL)
  {
    if ((t = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
    {
      error("Cannot malloc for t in id_add_param_info");
      return;
    }
    if(p->typ == NULL) {
      error("Cannot set type of param.");
      return;
    }
    end = p->typ->paratp;
    if(end != NULL){
      while(end->paratp != NULL){
        end = end->paratp;
      }
    }
    t->ttype = typetoken;
    //only standard type
    t->arraysize = 0;
    t->paratp = NULL;
    t->etp = NULL;
    end->paratp = t;
  }
  else
  {
    error("Cannnot find proc id data!");
    return;
  }
}

void id_add_reflinenum(char *np, int linenum)
{
  struct ID *p;
  struct LINE *l, *latest;
  if ((p = search_idtab(np)) != NULL)
  {
    if ((l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)
    {
      error("Cannot malloc for l in id_add_reflinenum");
      return;
    }
    latest = p->reflinep;
    if(latest != NULL){
      while(latest->nextlinep != NULL){
        latest = latest->nextlinep;
      }
    }
    l->linenum = linenum;
    l->nextlinep = NULL;
    latest->nextlinep = l;
  }
  else
  {
    error("Cannnot find array id data!");
    return;
  }
}

void id_add_reflinenum_local(char *np, int linenum)
{
  struct ID *p;
  struct LINE *l, *latest;
  if ((p = search_idtab_local(np)) != NULL)
  {
    if ((l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)
    {
      error("Cannot malloc for l in id_add_reflinenum");
      return;
    }
    latest = p->reflinep;
    if(latest != NULL){
      while(latest->nextlinep != NULL){
        latest = latest->nextlinep;
      }
    }
    l->linenum = linenum;
    l->nextlinep = NULL;
    latest->nextlinep = l;
  }
  else
  {
    error("Cannnot find array id data!");
    return;
  }
}

int search_variable_type(char *np){
  struct ID *iddatap = search_idtab(np);
  //NULL check
  if(iddatap == NULL) return S_ERROR;
  else if(iddatap->typ == NULL) return S_ERROR;

  return iddatap->typ->ttype;
}

int search_variable_type_local(char *np){
  struct ID *iddatap = search_idtab_local(np);
  //NULL check
  if(iddatap == NULL) return S_ERROR;
  else if(iddatap->typ == NULL) return S_ERROR;

  return iddatap->typ->ttype;
}

//mode get set
//for secure mode manage
void set_mode_local(){
  mode = LOCAL;
}

void set_mode_global(){
  mode = GLOBAL;
}

int get_mode(){
  return mode;
}
    /* void id_countup(char *np) {  Register and count up the name pointed by np
      struct ID *p;
      char *cp;
      if ((p = search_idtab(np)) != NULL)
        p->count++;
      else {
        if ((p = (struct ID *)malloc(sizeof(struct ID))) == NULL) {
          error("Cannot malloc for p in id_countup");
          return;
        }
        if ((cp = (char *)malloc(strlen(np) + 1)) == NULL) {
          error("Cannot malloc for cp in id_countup");
          return;
        }
        strcpy(cp, np);
        p->name = cp;
        p->count = 1;
        p->nextp = globalidroot;
        globalidroot = p;
      }
    } */

void print_idtab()
{ /* Output the registered data */
  struct ID *p;

  for (p = globalidroot; p != NULL; p = p->nextp)
  {
    printf("\t\"Identifier\" \"%s\"", p->name);
  }
}

void release_line(struct LINE *l)
{
  struct LINE *nlp;
  while (l != NULL)
  {
    nlp = l->nextlinep;
    free(l);
    l = nlp;
  }
}

void release_type(struct TYPE *t)
{
  struct TYPE *tmp;
  if(t == NULL) return;

  if (t->etp != NULL)
  {
    while (t->etp != NULL)
    {
      tmp = t->etp->etp;
      free(t->etp);
      t->etp = tmp;
    }
  }
  else if (t->paratp != NULL)
  {
    while (t->paratp != NULL)
    {
      tmp = t->paratp->paratp;
      free(t->paratp);
      t->paratp = tmp;
    }
  }
  free(t);
}

/* move local idtab to global idtab */
void clear_idtab_local()
{
  if(localidroot == NULL) return;
  localidroot->nextp = globalidroot;
  globalidroot = localidroot;
  localidroot = NULL;
}

void release_idtab()
{ /* Release tha data structure */
  struct ID *p, *q = NULL;
  clear_idtab_local();
  for (p = globalidroot; p != NULL; p = q)
  {
    free(p->name);
    if (p->processname != NULL)
      free(p->processname);
    if (p->reflinep != NULL)
      release_line(p->reflinep);
    release_type(p->typ);
    q = p->nextp;
    free(p);
  }
  init_idtab();
}
