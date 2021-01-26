#include "../inc/table.h"
#include "../inc/util.h"

#include <stddef.h>

typedef struct Binder_* Binder;
struct Binder_ {
    void* key;
    void* value;
    Binder tail;
};

#define TAB_SIZE 127

struct Table_ {
    Binder table[TAB_SIZE];
    Table tail;
};

Binder make_binder(void* key, void* value, Binder tail) {
    Binder b = check_malloc(sizeof(*b));
    b->key = key;
    b->value = value;
    b->tail = tail;
    return b;
}

Table init_tab(Table tail) {
    Table t = check_malloc(sizeof(*t));
    t->tail = tail;
    for (unsigned int i = 0; i < TAB_SIZE; ++i) {
        t->table[i] = NULL;
    }
    return t;
}

void* tab_look(Table t, void* key) {
    check(t && key);
    Binder head = t->table[(unsigned long long)key % TAB_SIZE];
    for (/*Intended left empty*/; head; head = head->tail) {
        if (head->key == key) return head->value;
    }
    return NULL;
}

void tab_insert(Table t, void* key, void* value) {
    check(t && key);
    if (tab_look(t, key) != NULL) {
        CHECK_WITH_MSG()
    }
    Binder head = t->table[(unsigned long long)key % TAB_SIZE];
    Binder b = make_binder(key, value, head);
    t->table[(unsigned long long)key % TAB_SIZE] = head;
}

void* tab_pop(Table t) {
    check(t);
    Binder top = t->top;
    check(top);
    void* key = top->key;
    check(key);
    t->table[(unsigned)key % TAB_SIZE] = top->tail;
    t->top = top->prevtop;
    return key;
}