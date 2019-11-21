#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "table.h"

typedef struct Symbol_* Symbol;
void init_symbol_tab();
Symbol make_symbol(const char* s);
const char* symbol_name(Symbol sb);

typedef struct Table_* S_table;
S_table S_empty();
void S_enter(S_table t, Symbol key, void* value);
void* S_look(S_table t, Symbol key);
void S_beginscope(S_table t);
void S_endscope(S_table t);

#endif