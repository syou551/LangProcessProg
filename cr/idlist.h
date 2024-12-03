
//type check

extern int search_variable_type(char *np);
extern int search_variable_type_local(char *np);
extern int get_mode();
extern void set_mode_local();
extern void set_mode_global();

extern void id_add_variable(char *np);
extern void id_add_info(char *np, int typetoken, int deflinenum);
extern void id_add_alement_info(char *np, int typetoken, int arraysize);