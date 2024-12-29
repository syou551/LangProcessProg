#include "makeCode.h"

static int labelcounter = 1;
static FILE *cslfp;

int get_new_label_num(void) {
    return labelcounter++;
}

void gen_code(char *code) {
    fprintf(cslfp, "\t%s\n", code);
}

void gen_code_label(char *code, int label) {
    fprintf(cslfp, "\t%s\tL%04d\n", code, label);
}

void gen_label(int label) { 
    fprintf(cslfp, "L%04d\tDS\t0\n", label);
}