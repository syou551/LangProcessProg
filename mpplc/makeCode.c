#include "makeCode.h"
#include "scan.h"

static int labelcounter = 1;
static struct LABEL *labelroot = NULL;


int gen_new_label_num(void) {
    return labelcounter++;
}

void gen_code(char *code,...) {
    va_list ap;
    va_start(ap, code);

    vfprintf(cslfp, code, ap);
    fprintf(cslfp,"\n");

    va_end(ap);
}

void print_code(char *code, ...){
    va_list ap;
    va_start(ap, code);

    fprintf(cslfp,"\t");
    vfprintf(cslfp, ap, code);

    va_end(ap); 
}

void print_code_linebreak(){
    fprintf(cslfp, "\n");
}

void gen_code_label(char *code, int label) {
    fprintf(cslfp, "\t%s\tL%04d\n", code, label);
}

void gen_label(int label) { 
    fprintf(cslfp, "L%04d\n", label);
}

char *get_symbol_keyword(int token){
    int i = 0;
    for(i = 0;i < KEYWORDSIZE;i++){
        if(token == key[i].keytoken){
            return key[i].keyword;
        }
    }
    for(i = 0;i < SYMBOLSIZE; i++){
        if(token == sym[i].symtoken){
            return sym[i].symbol;
            //return; comment out for C2 coverage 
        }
    }

    return NULL;
}

int push_label_list(int labelnum){
    if(labelroot == NULL){
        if((labelroot = malloc(sizeof(struct LABEL))) == NULL) return error("ERROR: can't get memory space for LABEL list");
        labelroot->labelnum = labelnum;
        labelroot->next = NULL;
    }else{
        struct LABEL *p = NULL;
        if((p = malloc(sizeof(struct LABEL))) == NULL) return error("ERROR: can't get memory space for LABEL list");
        p->labelnum = labelnum;
        p->next = labelroot;
        labelroot = p;
    }
    return 0;
}

int pop_label_list(){
    if(labelroot == NULL) return S_ERROR;
    int tmp = labelroot->labelnum;
    struct LABEL *p = labelroot;
    labelroot = labelroot->next;
    p->next = NULL;
    free(p);
    return tmp;
}

void outlib(void) {
  fprintf(cslfp,""
"; ------------------------\n"
"; Utility functions\n"
"; ------------------------\n"
"EOVF            CALL    WRITELINE\n"
"                LAD     gr1, EOVF1\n"
"                LD      gr2, gr0\n"
"                CALL    WRITESTR\n"
"                CALL    WRITELINE\n"
"                SVC     1  ;  overflow error stop\n"
"EOVF1           DC      '***** Run-Time Error : Overflow *****'\n"
"E0DIV           JNZ     EOVF\n"
"                CALL    WRITELINE\n"
"                LAD     gr1, E0DIV1\n"
"                LD      gr2, gr0\n"
"                CALL    WRITESTR\n"
"                CALL    WRITELINE\n"
"                SVC     2  ;  0-divide error stop\n"
"E0DIV1          DC      '***** Run-Time Error : Zero-Divide *****'\n"
"EROV            CALL    WRITELINE\n"
"                LAD     gr1, EROV1\n"
"                LD      gr2, gr0\n"
"                CALL    WRITESTR\n"
"                CALL    WRITELINE\n"
"                SVC     3  ;  range-over error stop\n"
"EROV1           DC      '***** Run-Time Error : Range-Over in Array Index *****'\n"
"; gr1の値（文字）をgr2のけた数で出力する．\n"
"; gr2が0なら必要最小限の桁数で出力する\n"
"WRITECHAR       RPUSH\n"
"                LD      gr6, SPACE\n"
"                LD      gr7, OBUFSIZE\n"
"WC1             SUBA    gr2, ONE  ; while(--c > 0) {\n"
"                JZE     WC2\n"
"                JMI     WC2\n"
"                ST      gr6, OBUF,gr7  ;  *p++ = ' ';\n"
"                CALL    BOVFCHECK\n"
"                JUMP    WC1  ; }\n"
"WC2             ST      gr1, OBUF,gr7  ; *p++ = gr1;\n"
"                CALL    BOVFCHECK\n"
"                ST      gr7, OBUFSIZE\n"
"                RPOP\n"
"                RET\n"
"; gr1が指す文字列をgr2のけた数で出力する．\n"
"; gr2が0なら必要最小限の桁数で出力する\n"
"WRITESTR        RPUSH\n"
"                LD      gr6, gr1  ; p = gr1;\n"
"WS1             LD      gr4, 0,gr6  ; while(*p != 0) {\n"
"                JZE     WS2\n"
"                ADDA    gr6, ONE  ;  p++;\n"
"                SUBA    gr2, ONE  ;  c--;\n"
"                JUMP    WS1  ; }\n"
"WS2             LD      gr7, OBUFSIZE  ; q = OBUFSIZE;\n"
"                LD      gr5, SPACE\n"
"WS3             SUBA    gr2, ONE  ; while(--c >= 0) {\n"
"                JMI     WS4\n"
"                ST      gr5, OBUF,gr7  ;  *q++ = ' ';\n"
"                CALL    BOVFCHECK\n"
"                JUMP    WS3  ; }\n"
"WS4             LD      gr4, 0,gr1  ; while(*gr1 != 0) {\n"
"                JZE     WS5\n"
"                ST      gr4, OBUF,gr7  ;  *q++ = *gr1++;\n"
"                ADDA    gr1, ONE\n"
"                CALL    BOVFCHECK\n"
"                JUMP    WS4  ; }\n"
"WS5             ST      gr7, OBUFSIZE  ; OBUFSIZE = q;\n"
"                RPOP\n"
"                RET\n"
"BOVFCHECK       ADDA    gr7, ONE\n"
"                CPA     gr7, BOVFLEVEL\n"
"                JMI     BOVF1\n"
"                CALL    WRITELINE\n"
"                LD      gr7, OBUFSIZE\n"
"BOVF1           RET\n"
"BOVFLEVEL       DC      256\n"
"; gr1の値（整数）をgr2のけた数で出力する．\n"
"; gr2が0なら必要最小限の桁数で出力する\n"
"WRITEINT        RPUSH\n"
"                LD      gr7, gr0  ; flag = 0;\n"
"                CPA     gr1, gr0  ; if(gr1>=0) goto WI1;\n"
"                JPL     WI1\n"
"                JZE     WI1\n"
"                LD      gr4, gr0  ; gr1= - gr1;\n"
"                SUBA    gr4, gr1\n"
"                CPA     gr4, gr1\n"
"                JZE     WI6\n"
"                LD      gr1, gr4\n"
"                LD      gr7, ONE  ; flag = 1;\n"
"WI1             LD      gr6, SIX  ; p = INTBUF+6;\n"
"                ST      gr0, INTBUF,gr6  ; *p = 0;\n"
"                SUBA    gr6, ONE  ; p--;\n"
"                CPA     gr1, gr0  ; if(gr1 == 0)\n"
"                JNZ     WI2\n"
"                LD      gr4, ZERO  ;  *p = '0';\n"
"                ST      gr4, INTBUF,gr6\n"
"                JUMP    WI5  ; }\n"
"; else {\n"
"WI2             CPA     gr1, gr0  ;  while(gr1 != 0) {\n"
"                JZE     WI3\n"
"                LD      gr5, gr1  ;   gr5 = gr1 - (gr1 / 10) * 10;\n"
"                DIVA    gr1, TEN  ;   gr1 /= 10;\n"
"                LD      gr4, gr1\n"
"                MULA    gr4, TEN\n"
"                SUBA    gr5, gr4\n"
"                ADDA    gr5, ZERO  ;   gr5 += '0';\n"
"                ST      gr5, INTBUF,gr6  ;   *p = gr5;\n"
"                SUBA    gr6, ONE  ;   p--;\n"
"                JUMP    WI2  ;  }\n"
"WI3             CPA     gr7, gr0  ;  if(flag != 0) {\n"
"                JZE     WI4\n"
"                LD      gr4, MINUS  ;   *p = '-';\n"
"                ST      gr4, INTBUF,gr6\n"
"                JUMP    WI5  ;  }\n"
"WI4             ADDA    gr6, ONE  ;  else p++;\n"
"; }\n"
"WI5             LAD     gr1, INTBUF,gr6  ; gr1 = p;\n"
"                CALL    WRITESTR  ; WRITESTR();\n"
"                RPOP\n"
"                RET\n"
"WI6             LAD     gr1, MMINT\n"
"                CALL    WRITESTR  ; WRITESTR();\n"
"                RPOP\n"
"                RET\n"
"MMINT           DC      '-32768'\n"
"; gr1の値（真理値）が0なら'FALSE'を\n"
"; 0以外なら'TRUE'をgr2のけた数で出力する．\n"
"; gr2が0なら必要最小限の桁数で出力する\n"
"WRITEBOOL       RPUSH\n"
"                CPA     gr1, gr0  ; if(gr1 != 0)\n"
"                JZE     WB1\n"
"                LAD     gr1, WBTRUE  ;  gr1 = TRUE;\n"
"                JUMP    WB2\n"
"; else\n"
"WB1             LAD     gr1, WBFALSE  ;  gr1 = FALSE;\n"
"WB2             CALL    WRITESTR  ; WRITESTR();\n"
"                RPOP\n"
"                RET\n"
"WBTRUE          DC      'TRUE'\n"
"WBFALSE         DC      'FALSE'\n"
"; 改行を出力する\n"
"WRITELINE       RPUSH\n"
"                LD      gr7, OBUFSIZE\n"
"                LD      gr6, NEWLINE\n"
"                ST      gr6, OBUF,gr7\n"
"                ADDA    gr7, ONE\n"
"                ST      gr7, OBUFSIZE\n"
"                OUT     OBUF, OBUFSIZE\n"
"                ST      gr0, OBUFSIZE\n"
"                RPOP\n"
"                RET\n"
"FLUSH           RPUSH\n"
"                LD      gr7, OBUFSIZE\n"
"                JZE     FL1\n"
"                CALL    WRITELINE\n"
"FL1             RPOP\n"
"                RET\n"
"; gr1が指す番地に文字一つを読み込む\n"
"READCHAR        RPUSH\n"
"                LD      gr5, RPBBUF  ; if(RPBBUF != 0) {\n"
"                JZE     RC0\n"
"                ST      gr5, 0,gr1  ;  *gr1 = RPBBUF;\n"
"                ST      gr0, RPBBUF  ;  RPBBUF = 0\n"
"                JUMP    RC3  ;  return; }\n"
"RC0             LD      gr7, INP  ; inp = INP;\n"
"                LD      gr6, IBUFSIZE  ; if(IBUFSIZE == 0) {\n"
"                JNZ     RC1\n"
"                IN      IBUF, IBUFSIZE  ;  IN();\n"
"                LD      gr7, gr0  ;  inp = 0;\n"
"; }\n"
"RC1             CPA     gr7, IBUFSIZE  ; if(inp == IBUFSIZE) {\n"
"                JNZ     RC2\n"
"                LD      gr5, NEWLINE  ;  *gr1 = '\\n';\n"
"                ST      gr5, 0,gr1\n"
"                ST      gr0, IBUFSIZE  ;  IBUFSIZE = INP = 0;\n"
"                ST      gr0, INP\n"
"                JUMP    RC3  ; }\n"
"; else {\n"
"RC2             LD      gr5, IBUF,gr7  ;  *gr1 = *inp++;\n"
"                ADDA    gr7, ONE\n"
"                ST      gr5, 0,gr1\n"
"                ST      gr7, INP  ;  INP = inp;\n"
"; }\n"
"RC3             RPOP\n"
"                RET\n"
"; gr1が指す番地に整数値一つを読み込む\n"
"READINT         RPUSH\n"
"; do {\n"
"RI1             CALL    READCHAR  ;  ch = READCHAR();\n"
"                LD      gr7, 0,gr1\n"
"                CPA     gr7, SPACE  ; } while(ch==' ' || ch=='\\t' || ch=='\\n');\n"
"                JZE     RI1\n"
"                CPA     gr7, TAB\n"
"                JZE     RI1\n"
"                CPA     gr7, NEWLINE\n"
"                JZE     RI1\n"
"                LD      gr5, ONE  ; flag = 1\n"
"                CPA     gr7, MINUS  ; if(ch == '-') {\n"
"                JNZ     RI4\n"
"                LD      gr5, gr0  ;  flag = 0;\n"
"                CALL    READCHAR  ;  ch = READCHAR();\n"
"                LD      gr7, 0,gr1\n"
"RI4             LD      gr6, gr0  ; v = 0;     ; }\n"
"RI2             CPA     gr7, ZERO  ; while('0' <= ch && ch <= '9') {\n"
"                JMI     RI3\n"
"                CPA     gr7, NINE\n"
"                JPL     RI3\n"
"                MULA    gr6, TEN  ;  v = v*10+ch-'0';\n"
"                ADDA    gr6, gr7\n"
"                SUBA    gr6, ZERO\n"
"                CALL    READCHAR  ;  ch = READSCHAR();\n"
"                LD      gr7, 0,gr1\n"
"                JUMP    RI2  ; }\n"
"RI3             ST      gr7, RPBBUF  ; ReadPushBack();\n"
"                ST      gr6, 0,gr1  ; *gr1 = v;\n"
"                CPA     gr5, gr0  ; if(flag == 0) {\n"
"                JNZ     RI5\n"
"                SUBA    gr5, gr6  ;  *gr1 = -v;\n"
"                ST      gr5, 0,gr1\n"
"; }\n"
"RI5             RPOP\n"
"                RET\n"
"; 入力を改行コードまで（改行コードも含む）読み飛ばす\n"
"READLINE        ST      gr0, IBUFSIZE\n"
"                ST      gr0, INP\n"
"                ST      gr0, RPBBUF\n"
"                RET\n"
"ONE             DC      1\n"
"SIX             DC      6\n"
"TEN             DC      10\n"
"SPACE           DC      #0020  ; ' '\n"
"MINUS           DC      #002D  ; '-'\n"
"TAB             DC      #0009  ; '\\t'\n"
"ZERO            DC      #0030  ; '0'\n"
"NINE            DC      #0039  ; '9'\n"
"NEWLINE         DC      #000A  ; '\\n'\n"
"INTBUF          DS      8\n"
"OBUFSIZE        DC      0\n"
"IBUFSIZE        DC      0\n"
"INP             DC      0\n"
"OBUF            DS      257\n"
"IBUF            DS      257\n"
"RPBBUF          DC      0\n"
  );
}