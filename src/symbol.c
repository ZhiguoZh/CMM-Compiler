#include "../inc/symbol.h"
#include "../inc/util.h"

#include <stddef.h>
#include <string.h>

struct Symbol_ {
    const char* name;
    Symbol tail;
};

#define HASH_SIZE 109

static Symbol hash_tab[HASH_SIZE];

static unsigned int hash(const char* s) {
    unsigned int hash_val = 0;
    for (/*Intended left empty*/; *s != NULL; ++s) {
        hash_val += hash_val * 65599 + *s;
    }
    return hash_val;
}

void init_symbol_tab() {
    for (unsigned int i = 0; i < HASH_SIZE; ++i) {
        hash_tab[i] = NULL;
    }
}

const char* symbol_name(Symbol sb) {
    return sb->name;
}

Symbol make_symbol(const char* s) {
    unsigned int index = hash(s) % HASH_SIZE;
    Symbol head = hash_tab[index], sb = head;
    for (/*Intended left empty*/; sb != NULL; sb = sb->tail) {
        if (strcmp(s, symbol_name(sb)) == 0) return sb;
    }
    sb = check_malloc(sizeof(*sb));
    sb->name = s;
    sb->tail = head;
    return sb;
}

S_table S_empty() {
    return init_tab();
}

void S_enter(S_table t, Symbol key, void* value) {
    tab_push(t, key, value);
}

void* S_look(S_table t, Symbol key) {
    return tab_look(t, key);
}

static struct Symbol_ scope_mark = {"<mark>", NULL};

void S_beginscope(S_table t) {
    S_enter(t, &scope_mark, NULL);
}

void S_endscope(S_table t) {
    Symbol key;
    do {
        key = tab_pop(t);
    } while(key != &scope_mark);
}