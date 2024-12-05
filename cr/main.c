#include "scan.h"
#include "parse.h"
#include "idlist.h"

// #region global var
/* keyword list */
struct KEY key[KEYWORDSIZE] = {
  {"and", TAND},         {"array", TARRAY},     {"begin", TBEGIN},
  {"boolean", TBOOLEAN}, {"break", TBREAK},     {"call", TCALL},
  {"char", TCHAR},       {"div", TDIV},         {"do", TDO},
  {"else", TELSE},       {"end", TEND},         {"false", TFALSE},
  {"if", TIF},           {"integer", TINTEGER}, {"not", TNOT},
  {"of", TOF},           {"or", TOR},           {"procedure", TPROCEDURE},
  {"program", TPROGRAM}, {"read", TREAD},       {"readln", TREADLN},
  {"return", TRETURN},   {"then", TTHEN},       {"true", TTRUE},
  {"var", TVAR},         {"while", TWHILE},     {"write", TWRITE},
  {"writeln", TWRITELN}};

/* symbol list */
struct SYM sym[SYMBOLSIZE] = {
  {"+", TPLUS}, {"-", TMINUS}, {"*", TSTAR},    {"=", TEQUAL},
  {"<>", TNOTEQ}, {"<", TLE},   {"<=", TLEEQ},   {">", TGR},
  {">=", TGREQ} , {"(", TLPAREN}, {")", TRPAREN},{"[", TLSQPAREN},
  {"]", TRSQPAREN},{":=", TASSIGN},{".", TDOT},  {",",  TCOMMA}, 
  {":", TCOLON},   {";", TSEMI}};

/* Token counter */
int numtoken[NUMOFTOKEN + 1];

/* string of each token */
char *tokenstr[NUMOFTOKEN + 1] = {
  "",        "NAME",    "program", "var",     "array",     "of",     
  "begin",   "end",     "if",      "then",    "else",      "procedure",
  "return",  "call",    "while",   "do",      "not",       "or",
  "div",     "and",     "char",    "integer", "boolean",   "readln",
  "writeln", "true",    "false",   "NUMBER",  "STRING",    "+",
  "-",       "*",       "=",       "<>",      "<",         "<=",
  ">",       ">=",      "(",       ")",       "[",         "]",
  ":=",      ".",       ",",       ":",       ";",         "read",   
  "write",   "break"};


// #endregion

int main(int nc, char *np[]) {
  int token, i;

  if (nc < 2) {
    error("File name is not given.");
    return 0;
  }
  if (init_scan(np[1]) < 0) {
    error("Cannot open input file.");
    return 0;
  }
  init_idtab();
  /* 作成する部分：構文解析を行う */
  parse_program();

  //id table output
  print_idtab();

  end_scan();
  return 0;
}

int error(char *mes,...) {
  va_list ap;
  va_start(ap, mes);

  fprintf(stderr, "\nLine: %4d ", get_linenum());
  vfprintf(stderr,mes, ap);
  fprintf(stderr,"\n");
  return S_ERROR;
}