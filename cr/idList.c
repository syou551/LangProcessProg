#include "parse.h"
#include "idlist.h"
#include <stdlib.h>


static int mode = GLOBAL;
static char *processname = NULL;
static int column_size[COLUMNNUM] = {0,11+11,0,0};

struct ID *globalidroot, *localidroot;

void init_idtab()
{ /* Initialise the table */
  globalidroot = NULL;
  localidroot = NULL;
}

struct ID *search_idtab(char *np)
{ /* search the name pointed by np */
  struct ID *p = NULL;

  for (p = globalidroot; p != NULL; p = p->nextp)
  {
    if (strcmp(np, p->name) == 0)
      if (!(p->ispara)) return p;
  }
  return NULL;
}

struct ID *search_idtab_local(char *np)
{
  struct ID *p;
  for (p = localidroot; p != NULL; p = p->nextp)
  {
    if (strcmp(np, p->name) == 0) return (p);
  }
  return (NULL);
}

int id_add_variable(char *np, int deflinenum)
{
  struct ID *p;
  struct TYPE *t;
  struct LINE *l;
  char *cp;
  if ((p = (struct ID *)malloc(sizeof(struct ID))) == NULL) return error("Cannot malloc for p in id_add_info");
  if ((cp = (char *)malloc(strlen(np) + 1)) == NULL) return error("Cannot malloc for cp in id_add_info");
  if ((t = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) return error("Cannot malloc for t in id_add_info");
  if ((l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL) return error("Cannot malloc for l in id_add_info");
  
  strcpy(cp, np);
  p->name = cp;
  p->processname = NULL;
  p->ispara = get_mode();

  p->typ = t;
  p->deflinenum = deflinenum;
  p->nextp = NULL;
  p->reflinep = NULL;
  // check conflict and add to table
  if (get_mode() == GLOBAL){
    if (search_idtab(np) != NULL) return error("ERROR: this name \"%s\" variable has been declared", np);
    p->nextp = globalidroot;
    globalidroot = p;
  }else if (get_mode() == LOCAL){
    if (search_idtab_local(np) != NULL) return error("ERROR: this name \"%s\" variable has been declared", np);
    p->nextp = localidroot;
    localidroot = p;
  }
  t->ttype = S_ERROR;
  t->arraysize = 0;
  t->etp = NULL;
  t->paratp = NULL;

  return 0;
}

int id_add_info(char *np, int typetoken)
{
  struct ID *p = NULL;
  switch (mode)
  {
    case 0:
      p = search_idtab(np);
      break;
    case 1:
      return id_add_info_local(np, processname, typetoken);
  }
  if (p == NULL) return error("ERROR: internal error(variable isn't added table)");
  struct TYPE *t = p->typ;
  struct LINE *l = p->reflinep;
  char *cp;

  t->ttype = typetoken;
  t->arraysize = 0;
  t->etp = NULL;
  t->paratp = NULL;

  return 0;
}
// call from id_add_info
int id_add_info_local(char *np, char *processnp, int typetoken)
{
  struct ID *p = NULL;
  if ((p = search_idtab_local(np)) == NULL) return error("ERROR: internal error(variable isn't added table)");

  struct TYPE *t = p->typ;
  struct LINE *l = p->reflinep;
  char *procp;
  if ((procp = (char *)malloc(strlen(processnp) + 1)) == NULL) return error("Cannot malloc for procp in id_add_info_local");

  strcpy(procp, processnp);
  p->processname = procp;
  p->ispara = 1;
  t->ttype = typetoken;
  t->arraysize = 0;
  t->etp = NULL;
  t->paratp = NULL;

  return 0;
}

int id_add_element_info(char *np, int typetoken, int arraysize)
{
  struct ID *p;
  struct TYPE *t;
  switch (mode)
  {
    case 0:
      p = search_idtab(np);
      break;
    case 1:
      return id_add_element_info_local(np, typetoken, arraysize);
  }
  if (p != NULL)
  {
    if ((t = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) return error("Cannot malloc for t in id_add_element_info");

    p->typ->etp = t;
    p->typ->arraysize = arraysize;
    t->ttype = typetoken;
    t->arraysize = 0;
    t->etp = NULL;
    t->paratp = NULL;
  }else return error("Cannnot find array id data!");

  return 0;
}
// call from id_add_element_info
int id_add_element_info_local(char *np, int typetoken, int arraysize)
{
  struct ID *p;
  struct TYPE *t;
  if ((p = search_idtab_local(np)) != NULL)
  {
    if ((t = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) return error("Cannot malloc for t in id_add_element_info");

    p->typ->etp = t;
    p->typ->arraysize = arraysize;
    t->ttype = typetoken;
    t->arraysize = 0;
    t->etp = NULL;
    t->paratp = NULL;
  }else return error("Cannnot find array id data!");

  return 0;
}

int id_add_param_info(int typetoken)
{
  struct ID *p;
  struct TYPE *t, *end;
  if ((p = search_idtab(processname)) != NULL)
  {
    if ((t = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) return error("Cannot malloc for t in id_add_param_info");
    if (p->typ == NULL) return error("Cannot set type of param.");

    end = p->typ->paratp;
    if (end != NULL)
    {
      while (end->paratp != NULL)
      {
        end = end->paratp;
      }
    }
    t->ttype = typetoken;
    // only standard type
    t->arraysize = 0;
    t->paratp = NULL;
    t->etp = NULL;
    if (end == NULL) p->typ->paratp = t;
    else end->paratp = t;
  }else return error("Cannnot find proc id data!");

  return 0;
}

// while compound state process.
int id_add_reflinenum(char *np, int linenum)
{
  switch (mode)
  {
    case LOCAL:
      return id_add_reflinenum_local(np, linenum);
    case GLOBAL:
      break;
  }
  struct ID *p = NULL;
  struct LINE *l = NULL, *latest = NULL;

  if ((p = search_idtab(np)) != NULL)
  {
    if ((l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL) return error("Cannot malloc for l in id_add_reflinenum");

    latest = p->reflinep;

    if (latest != NULL)
    {
      while (latest->nextlinep != NULL)
      {
        latest = latest->nextlinep;
      }
    }
    l->linenum = linenum;
    l->nextlinep = NULL;
    if (latest == NULL) p->reflinep = l;
    else latest->nextlinep = l;
  }else return error("Cannnot find variable id data!");

  return 0;
}

int id_add_reflinenum_local(char *np, int linenum)
{
  struct ID *p;
  struct LINE *l, *latest;
  if ((p = search_idtab_local(np)) == NULL)
  {
    if ((p = search_idtab(np)) == NULL) return error("Cannnot find variable id data!");
  }

  if ((l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)return error("Cannot malloc for l in id_add_reflinenum");

  latest = p->reflinep;
  if (latest != NULL)
  {
    while (latest->nextlinep != NULL)
    {
      latest = latest->nextlinep;
    }
  }
  l->linenum = linenum;
  l->nextlinep = NULL;
  if (latest == NULL) p->reflinep = l;
  else latest->nextlinep = l;

  return 0;
}

int search_variable_type(char *np)
{
  int typetoken;
  switch (mode)
  {
  case LOCAL:
    return search_variable_type_local(np);
  case GLOBAL:
    break;
  }
  struct ID *iddatap = search_idtab(np);
  // NULL check
  if (iddatap == NULL) return error("ERROR: \"%s\" isn't delared", np);
  else if (iddatap->typ == NULL) return error("ERROR: varibale type struct didn't maked");
  else if((typetoken = iddatap->typ->ttype) == S_ERROR) return error("ERROR: varibale type not registar");

  return typetoken;
}

int search_variable_type_local(char *np)
{
  struct ID *iddatap = search_idtab_local(np);
  int typetoken;
  // NULL check
  if (iddatap == NULL)
  {
    iddatap = search_idtab(np);
    if (iddatap == NULL) return error("ERROR: \"%s\" isn't delared", np);
  }
  if (iddatap->typ == NULL) return error("ERROR: varibale type struct didn't maked");
  else if((typetoken = iddatap->typ->ttype) == S_ERROR) return error("ERROR: varibale type not registar");

  return typetoken;
}

int search_array_element_type(char *np)
{
  int typetoken;
  switch (mode)
  {
  case LOCAL:
    return search_array_element_type_local(np);
  case GLOBAL:
    break;
  }
  struct ID *iddatap = search_idtab(np);
  // NULL check
  if (iddatap == NULL) return error("ERROR: \"%s\" isn't delared", np);
  else if (iddatap->typ == NULL) return error("ERROR: \"%s\" type info not found", np);
  else if (iddatap->typ->etp == NULL) return error("ERROR: \"%s\" element type isn't delared", np);
  else if ((typetoken = iddatap->typ->etp->ttype) == S_ERROR) return error("ERROR: \"%s\" element type isn't delared", np);
  
  return typetoken;
}

int search_array_element_type_local(char *np)
{
  struct ID *iddatap = search_idtab_local(np);
  if(iddatap == NULL){
    iddatap = search_idtab(np);
    if (iddatap == NULL) return error("ERROR: \"%s\" isn't declared", np);
  }
  int typetoken;
  // NULL check
  if (iddatap == NULL) return error("ERROR: \"%s\" isn't delared", np);
  else if (iddatap->typ == NULL) return error("ERROR: \"%s\" type info not found", np);
  else if (iddatap->typ->etp == NULL) return error("ERROR: \"%s\" element type isn't delared", np);
  else if ((typetoken = iddatap->typ->etp->ttype) == S_ERROR) return error("ERROR: \"%s\" element type isn't delared", np);
  
  return typetoken;
}

int search_array_size(char *np)
{
  switch (mode)
  {
  case LOCAL:
    return search_array_size_local(np);
  case GLOBAL:
    break;
  }
  struct ID *iddatap = search_idtab(np);
  // NULL check
  if (iddatap == NULL) return error("ERROR: \"%s\" isn't delared", np);
  else if (iddatap->typ == NULL) return error("ERROR: \"%s\" type info not found", np);
  
  return iddatap->typ->arraysize;
}

int search_array_size_local(char *np)
{
  struct ID *iddatap = search_idtab_local(np);
  if(iddatap == NULL){
    iddatap = search_idtab(np);
    if (iddatap == NULL) return error("ERROR: \"%s\" isn't declared", np);
  }
  // NULL check
  if (iddatap == NULL) return error("ERROR: \"%s\" isn't delared", np);
  else if (iddatap->typ == NULL) return error("ERROR: \"%s\" type info not found", np);
  
  return iddatap->typ->arraysize;
}

// mode get set
// for secure mode manage
int set_mode_local(char *np)
{
  processname = NULL;

  if ((processname = (char *)malloc(strlen(np) + 1)) == NULL) return error("ERROR: failure get space of processname");

  strcpy(processname, np);
  mode = LOCAL;
  return 0;
}

void set_mode_global()
{
  mode = GLOBAL;
  if (processname != NULL) free(processname);
  clear_idtab_local();
}

int get_mode()
{
  return mode;
}

char *get_processname()
{
  return processname;
}

void check_column_size(struct ID *p){
  struct ID *idp;
  for(idp = p; idp != NULL; idp = idp->nextp){
    if(idp->ispara) column_size[0] =
     (column_size[0] < (strlen(idp->name) + 1 + strlen(idp->processname))) 
      ? strlen(idp->name) + 1 + strlen(idp->processname) 
      : column_size[0];
    else column_size[0] =
     (column_size[0] < strlen(idp->name)) 
      ? strlen(idp->name) 
      : column_size[0];
  }
}

void print_idtab()
{ /* Output the registered data */
  struct ID *p;
  // print header
  if(globalidroot != NULL) {
    globalidroot = sort_idtab(globalidroot);
    check_column_size(globalidroot);
  }
  printf("Name");
  for(int i = 0;i <= (column_size[0] / 4);i++){
    printf("\t");
  }
  printf("| Type");
  for(int i = 0;i < (column_size[1] / 4) ;i++){
    printf("\t");
  }
  printf("| Define| References\n");
  fflush(stdout);
  

  for (p = globalidroot; p != NULL; p = p->nextp)
  {
    if (p->ispara)
    {
      printf("%s:%s", p->name, p->processname);
      for(int i = 0;i < (column_size[0]/4 + 1) - ((strlen(p->name) + 1 + strlen(p->processname))/4);i++){
        printf("\t");
      }
      printf("|");
    }
    else{
      printf("%s", p->name);
      for(int i = 0;i < (column_size[0]/4 + 1) - (strlen(p->name)/4);i++){
        printf("\t");
      }
      printf("|");
    }
    
    if (p->typ->ttype == TARRAY)
    {
      printf(" array[%d] of %s", p->typ->arraysize, tokenstr[p->typ->etp->ttype]);
      for(int i = 0;i < (column_size[1]/4 + 1) - ((11 + strlen(tokenstr[p->typ->etp->ttype]))/4);i++){
        printf("\t");
      }
      printf("|");
    }
    else if (p->typ->ttype == TPROCEDURE)
    {
      if (p->typ->paratp == NULL) printf(" procedure\t\t\t\t|");
      else
      {
        struct TYPE *tp;
        tp = p->typ->paratp;
        printf(" procedure(");
        int count = 4;
        while (tp != NULL)
        {
          printf("%s", tokenstr[tp->ttype]);
          if (tp->paratp != NULL)
            printf(", ");
          tp = tp->paratp;
          count--;
        }
        printf(")");
        for(int i = 0;i < count;i++){
          printf("\t");
        }
        printf("|");
      }
    }
    else{
      printf(" %s", tokenstr[p->typ->ttype]);
      switch(p->typ->ttype){
        case TCHAR : printf("\t\t\t\t\t");break;
        default: printf("\t\t\t\t");break;
      }
      printf("|");
    }

    printf(" %d\t|", p->deflinenum);
    // reflinenum
    struct LINE *lp;
    lp = p->reflinep;
    while (lp != NULL)
    {
      printf(" %d", lp->linenum);
      if (lp->nextlinep != NULL)
        printf(",");
      lp = lp->nextlinep;
    }
    printf("\n");
  }
}



struct ID *sort_idtab(struct ID *p)
{
  if(p->nextp == NULL) return p;
  
  struct ID  *m,*n,*new = NULL, *root = NULL;
  m = p;
  n = p->nextp;
  while(n->nextp != NULL){
    m = m->nextp;
    n = n->nextp;
    if(n->nextp == NULL) break;
    n = n->nextp;
  }
  n = m->nextp;
  m->nextp = NULL;
  m = sort_idtab(p);
  n = sort_idtab(n);

  while(m != NULL || n != NULL){
    if(m == NULL){
      new->nextp = n;
      new = n;
      n = n->nextp;
    }else if(n == NULL){
      new->nextp = m;
      new = m;
      m = m->nextp;
    }else if(strcmp(m->name,n->name) < 0){
      if(new == NULL) {
        new = m;
        root = new;
      }else{
        new->nextp = m;
        new = m;
      }
      m = m->nextp;
    }else{
      if(new == NULL) {
        new = n;
        root = new;
      }else{
        new->nextp = n;
        new = n;
      } 
      n = n->nextp;
    }
  }

  return root;
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
  if (t == NULL) return;

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
  if (localidroot == NULL) return;
  struct ID *p;
  p = localidroot;
  while(p->nextp != NULL){
    p = p->nextp;
  }
  p->nextp = globalidroot;
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
