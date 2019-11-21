#include "../inc/table.h"
#include "../inc/util.h"

#include <stddef.h>

typedef struct Binder_* Binder;
struct Binder_ {
    void* key;
    void* value;
    Binder tail;
    Binder prevtop;
};

#define TAB_SIZE 127

struct Table_ {
    Binder table[TAB_SIZE];
    Binder top;
};

Binder make_binder(void* key, void* value, Binder tail, Binder prevtop) {
    Binder b = check_malloc(sizeof(*b));
    b->key = key;
    b->value = value;
    b->tail = tail;
    b->prevtop = prevtop;
    return b;
}

Table init_tab() {
    Table t = check_malloc(sizeof(*t));
    t->top = NULL;
    for (unsigned int i = 0; i < TAB_SIZE; ++i) {
        t->table[i] = NULL;
    }
    return t;
}

void tab_push(Table t, void* key, void* value) {
    check(t && key);
    Binder head = t->table[(unsigned long long)key % TAB_SIZE];
    Binder b = make_binder(key, value, head, t->top);
    t->table[(unsigned long long)key % TAB_SIZE] = head;
    t->top = b;
}

void* tab_look(Table t, void* key) {
    check(t && key);
    Binder head = t->table[(unsigned long long)key % TAB_SIZE];
    return head->value;
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