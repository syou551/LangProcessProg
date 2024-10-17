#include "scan.h"

// init counter array of token
void init_counter(int *counter_array){
    int i = 0;
    for(i = 0;i < NUMOFTOKEN + 1; i++){
        counter_array[i] = 0;
    }
    init_idtab();
}

// print result of count
void print_numtoken(int *counter_array, char **tokenstr){
    int i  = 0;
    for(i = 1;i < NUMOFTOKEN + 1;i++){
        if(counter_array[i] == 0) continue;
        printf("\"%s\"\t%d\n",tokenstr[i],counter_array[i]);
        if(i == TNAME) print_idtab();
    }
    
    printf("%d\n",get_linenum());
}