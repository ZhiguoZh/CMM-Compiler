#include "../inc/absyntax.h"

#include <stdio.h>

A_program program_root = NULL;

void* check_malloc(int sz) {
    void* ptr = malloc(sz);
    if (ptr == NULL) {
        fprintf(stderr, "Error: Run out of Memory");
        exit(1);
    }
    return ptr;
}

// A.1.2 High-level definitions.
A_program A_Program(A_pos pos, A_extdeflist extdeflist) {
    A_program p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->extdeflist = extdeflist;
    return p;
}

A_extdeflist A_Extdeflist(A_pos pos, A_extdef extdef, A_extdeflist tail) {
    A_extdeflist p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->extdef = extdef;
    p->tail = tail;
    return p;
}

A_extdef A_Extdef_Var(A_pos pos, A_specifier type, A_extdeclist varlist) {
    A_extdef p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_extdef_var;
    p->u.var.type = type;
    p->u.var.varlist = varlist;
    return p;
}

A_extdef A_Extdef_Struct(A_pos pos, A_specifier struct_type) {
    A_extdef p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_extdef_struct;
    p->u.struct_type = struct_type;
    return p;
}

A_extdef A_Extdef_Func(A_pos pos, A_specifier ret_type, A_fundec fundec, A_compst body) {
    A_extdef p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_extdef_func;
    p->u.func.ret_type = ret_type;
    p->u.func.fundec = fundec;
    p->u.func.body = body;
    return p;
}

A_extdeclist A_Extdeclist(A_pos pos, A_vardec vardec, A_extdeclist tail) {
    A_extdeclist p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->vardec = vardec;
    p->tail = tail;
    return p;
}
// A.1.3 Specifiers.
A_specifier A_Builtin_Spec(A_pos pos, A_type builtin) {
    A_specifier p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_builtin_spec;
    p->u.builtin = builtin;
    return p;
}

A_specifier A_Struct_Spec(A_pos pos, A_struct_specifier structt) {
    A_specifier p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_struct_spec;
    p->u.structt = structt;
    return p;
}

A_struct_specifier A_Struct_Def(A_pos pos, A_opt_tag opttag, A_deflist deflist) {
    A_struct_specifier p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_struct_def;
    p->u.struct_def.opttag = opttag;
    p->u.struct_def.deflist = deflist;
    return p;
}

A_struct_specifier A_Struct_Dec(A_pos pos, A_tag tag) {
    A_struct_specifier p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_struct_dec;
    p->u.tag = tag;
    return p;
}

A_tag A_Tag(A_pos pos, A_id id) {
    A_tag p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->id = id;
    return p;
}

A_opt_tag A_Opt_Tag(A_pos pos, A_id id) {
    A_opt_tag p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->id = id;
    return p;
}

// A.1.4 Declarators.
A_vardec A_Id_Var(A_pos pos, A_id id) {
    A_vardec p = check_malloc(sizeof(*p));
    p->kind = A_id_var;
    p->pos = pos;
    p->u.id = id;
    return p;
}

A_vardec A_Arr_Var(A_pos pos, A_vardec vardec, int len) {
    A_vardec p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_arr_var;
    p->u.arr.vardec = vardec;
    p->u.arr.len = len;
    return p;
}

A_fundec A_Fundec(A_pos pos, A_id name, A_varlist varlist) {
    A_fundec p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->name = name;
    p->varlist = varlist;
    return p;
}

A_varlist A_Varlist(A_pos pos, A_paramdec param, A_varlist tail) {
    A_varlist p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->param = param;
    p->tail = tail;
    return p;
}

A_paramdec A_Paramdec(A_pos pos, A_specifier type, A_vardec vardec) {
    A_paramdec p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->type = type;
    p->vardec = vardec;
    return p;
}
// A.1.5 Statements.
A_compst A_Compst(A_pos pos, A_deflist deflist, A_stmtlist stmtlist) {
    A_compst p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->deflist = deflist;
    p->stmtlist = stmtlist;
    return p;
}

A_stmtlist A_Stmtlist(A_pos pos, A_stmt stmt, A_stmtlist tail) {
    A_stmtlist p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->stmt = stmt;
    p->tail = tail;
    return p;
}

A_stmt A_Exp_Stmt(A_pos pos, A_exp exp) {
    A_stmt p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_exp_stmt;
    p->u.exp = exp;
    return p;
}

A_stmt A_Comp_Stmt(A_pos pos, A_compst compst) {
    A_stmt p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_comp_stmt;
    p->u.compst = compst;
    return p;
}

A_stmt A_Ret_Stmt(A_pos pos, A_exp retexp) {
    A_stmt p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_ret_stmt;
    p->u.retExp = retexp;
    return p;
}

A_stmt A_If_Stmt(A_pos pos, A_exp cond, A_stmt if_stmt) {
    A_stmt p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_if_stmt;
    p->u.iff.cond = cond;
    p->u.iff.if_stmt = if_stmt;
    return p;
}

A_stmt A_If_Else_Stmt(A_pos pos, A_exp cond, A_stmt if_stmt, A_stmt else_stmt) {
    A_stmt p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_if_else_stmt;
    p->u.ifelse.cond = cond;
    p->u.ifelse.if_stmt = if_stmt;
    p->u.ifelse.else_stmt = else_stmt;
    return p;
}

A_stmt A_While_Stmt(A_pos pos, A_exp cond, A_stmt body) {
    A_stmt p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_while_stmt;
    p->u.whilee.cond = cond;
    p->u.whilee.body = body;
    return p;
}
// A.1.6 Local definitions.
A_def A_Def(A_pos pos, A_specifier type, A_declist declist) {
    A_def p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->type = type;
    p->declist = declist;
    return p;
}

A_deflist A_Deflist(A_pos pos, A_def def, A_deflist tail) {
    A_deflist p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->def = def;
    p->tail = tail;
    return p;
}

A_dec A_Dec(A_pos pos, A_vardec var, A_exp exp) {
    A_dec p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->var = var;
    p->exp = exp;
    return p;
}

A_declist A_Declist(A_pos pos, A_dec dec, A_declist tail) {
    A_declist p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->dec = dec;
    p->tail = tail;
    return p;
}

// A.1.7 Expressions.
A_exp A_Bop_Exp(A_pos pos, A_bop op, A_exp left, A_exp right) {
    A_exp p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_bop_exp;
    p->u.bop.op = op;
    p->u.bop.left = left;
    p->u.bop.right = right;
    return p;
}

A_exp A_Uop_Exp(A_pos pos, A_uop op, A_exp exp) {
    A_exp p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_uop_exp;
    p->u.uop.op = op;
    p->u.uop.exp = exp;
    return p;
}

A_exp A_Call_Exp(A_pos pos, A_id fun, A_args args) {
    A_exp p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_call_exp;
    p->u.call.fun = fun;
    p->u.call.args = args;
    return p;
}

A_exp A_Array_Exp(A_pos pos, A_exp arr, A_exp index) {
    A_exp p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_array_exp;
    p->u.array.arr = arr;
    p->u.array.index = index;
    return p;
}

A_exp A_Struct_Exp(A_pos pos, A_exp record, A_id field) {
    A_exp p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_struct_exp;
    p->u.structt.record = record;
    p->u.structt.field = field;
    return p;
}

A_exp A_Id_Exp(A_pos pos, A_id id) {
    A_exp p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_id_exp;
    p->u.id = id;
    return p;
}

A_exp A_Int_Exp(A_pos pos, int intt) {
    A_exp p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_int_exp;
    p->u.intt = intt;
    return p;
}

A_exp A_Float_Exp(A_pos pos, float floatt) {
    A_exp p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->kind = A_float_exp;
    p->u.floatt = floatt;
    return p;
}

A_args A_Args(A_pos pos, A_exp exp, A_args tail) {
    A_args p = check_malloc(sizeof(*p));
    p->pos = pos;
    p->exp = exp;
    p->tail = tail;
    return p;
}