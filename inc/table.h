#ifndef TABLE_H_
#define TABLE_H_

typedef struct Table_* Table;
Table init_tab();
void  tab_push(Table t, void* key, void* value);
void* tab_look(Table t, void* key);
void* tab_pop(Table t);
#endif