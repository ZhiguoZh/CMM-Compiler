#ifndef ABSYNTAX_H_
#define ABSYNTAX_H_

#include "symbol.h"

typedef struct A_program_* A_program;
typedef struct A_extdeflist_* A_extdeflist;
typedef struct A_extdef_* A_extdef;
typedef struct A_extdeclist_* A_extdeclist;
typedef struct A_struct_specifier_* A_struct_specifier;
typedef struct A_opt_tag_* A_opt_tag;
typedef struct A_tag_* A_tag;
typedef struct A_fundec_* A_fundec;
typedef struct A_varlist_* A_varlist;
typedef struct A_paramdec_* A_paramdec;
typedef struct A_compst_* A_compst;
typedef struct A_stmtlist_* A_stmtlist;
typedef struct A_stmt_* A_stmt;
typedef struct A_specifier_* A_specifier;
typedef struct A_deflist_* A_deflist;
typedef struct A_def_* A_def;
typedef struct A_declist_* A_declist;
typedef struct A_dec_* A_dec;
typedef struct A_vardec_* A_vardec;
typedef struct A_exp_* A_exp;
typedef struct A_args_* A_args;
typedef Symbol A_id;
typedef enum {A_int_type, A_float_type} A_type;
typedef int A_pos;

// A.1.2 High-level definitions.
struct A_program_ {
    A_pos pos;
    A_extdeflist extdeflist;
};

struct A_extdeflist_ {
    A_pos pos;
    A_extdef extdef;
    A_extdeflist tail;
};

struct A_extdef_ {
    A_pos pos;
    enum {A_extdef_var, A_extdef_struct, A_extdef_func} kind;
    union {
        struct {
            A_specifier type;
            A_extdeclist varlist;
        } var;
        A_specifier struct_type;
        struct {
            A_specifier ret_type;
            A_fundec fundec;
            A_compst body;
        } func;
    } u;
};

struct A_extdeclist_ {
    A_pos pos;
    A_vardec vardec;
    A_extdeclist tail;
};

// A.1.3 Specifiers.
struct A_specifier_ {
    A_pos pos;
    enum {A_builtin_spec, A_struct_spec} kind;
    union
    {
        A_type builtin;
        A_struct_specifier structt; 
    }u;
};

struct A_struct_specifier_ {
    A_pos pos;
    enum {A_struct_def, A_struct_dec} kind;
    union {
        struct {
            A_opt_tag opttag;
            A_deflist deflist;
        } struct_def;
        A_tag tag;
    } u;
};

struct A_opt_tag_ {
    A_pos pos;
    A_id id;
};

struct A_tag_ {
    A_pos pos;
    A_id id;
};
// A.1.4 Declarators.
struct A_vardec_ {
    A_pos pos;
    enum {A_id_var, A_arr_var} kind;
    union 
    {
        A_id id;
        struct
        {
            A_vardec vardec;
            int len;
        } arr;
    }u;
};

struct A_fundec_ {
    A_pos pos;
    A_id name;
    A_varlist varlist;
};

struct A_varlist_ {
    A_pos pos;
    A_paramdec param;
    A_varlist tail;
};

struct A_paramdec_ {
    A_pos pos;
    A_specifier type;
    A_vardec vardec;
};

// A.1.5 Statements.
struct A_compst_ {
    A_pos pos;
    A_deflist deflist;
    A_stmtlist stmtlist;
};

struct A_stmtlist_ {
    A_pos pos;
    A_stmt stmt;
    A_stmtlist tail;
};

struct A_stmt_ {
    A_pos pos;
    enum {A_exp_stmt, A_comp_stmt, A_ret_stmt, A_if_stmt, A_if_else_stmt, A_while_stmt} kind;
    union
    {
        A_exp exp;
        A_compst compst;
        A_exp retExp;
        struct
        {
            A_exp cond;
            A_stmt if_stmt;
        } iff;
        struct
        {
            A_exp cond;
            A_stmt if_stmt;
            A_stmt else_stmt;
        } ifelse;
        struct
        {
            A_exp cond;
            A_stmt body;
        } whilee;
    } u;
};
// A.1.6 Local definitions.
struct A_def_ {
    A_pos pos;
    A_specifier type;
    A_declist declist;
};

struct A_deflist_ {
    A_pos pos;
    A_def def;
    A_deflist tail;
};

struct A_dec_ {
    A_pos pos;
    A_vardec var;
    A_exp exp;
};

struct A_declist_ {
    A_pos pos;
    A_dec dec;
    A_declist tail;
};

// A.1.7 Expressions.
typedef enum {A_assignOp, A_andOp, A_orOp, A_gtOp, A_geOp, A_ltOp, A_leOp, A_eqOp, A_neOp, A_plusOp, A_minusOp, A_starOp, A_divOp} A_bop;
typedef enum {A_parOp, A_notOp, A_uminusOp} A_uop;

struct A_exp_ {
    A_pos pos;
    enum {A_bop_exp, A_uop_exp, A_call_exp, A_array_exp, A_struct_exp, A_id_exp, A_int_exp, A_float_exp} kind;
    union 
    {
        struct {
            A_bop op;
            A_exp left;
            A_exp right;
        } bop;
        struct {
            A_uop op;
            A_exp exp;
        } uop;
        struct {
            A_id fun;
            A_args args;
        } call;
        struct {
            A_exp arr;
            A_exp index;
        } array;
        struct {
            A_exp record;
            A_id field;
        } structt;
        A_id id;
        int intt;
        float floatt;
    } u;
};

struct A_args_ {
    A_pos pos;
    A_exp exp;
    A_args tail;    
};

// Constructors.
// A.1.2 High-level definitions.
A_program A_Program(A_pos pos, A_extdeflist extdeflist);
A_extdeflist A_Extdeflist(A_pos pos, A_extdef extdef, A_extdeflist tail);
A_extdef A_Extdef_Var(A_pos pos, A_specifier type, A_extdeclist varlist);
A_extdef A_Extdef_Struct(A_pos pos, A_specifier struct_type);
A_extdef A_Extdef_Func(A_pos pos, A_specifier ret_type, A_fundec fundec, A_compst body);
A_extdeclist A_Extdeclist(A_pos pos, A_vardec vardec, A_extdeclist tail);
// A.1.3 Specifiers.
A_specifier A_Builtin_Spec(A_pos pos, A_type builtin);
A_specifier A_Struct_Spec(A_pos pos, A_struct_specifier structt);
A_struct_specifier A_Struct_Def(A_pos pos, A_opt_tag opttag, A_deflist deflist);
A_struct_specifier A_Struct_Dec(A_pos pos, A_tag tag);
A_tag A_Tag(A_pos pos, A_id id);
A_opt_tag A_Opt_Tag(A_pos pos, A_id id);
// A.1.4 Declarators.
A_vardec A_Id_Var(A_pos pos, A_id id);
A_vardec A_Arr_Var(A_pos pos, A_vardec vardec, int len);
A_fundec A_Fundec(A_pos pos, A_id name, A_varlist varlist);
A_varlist A_Varlist(A_pos pos, A_paramdec para, A_varlist tail);
A_paramdec A_Paramdec(A_pos pos, A_specifier type, A_vardec vardec);
// A.1.5 Statements.
A_compst A_Compst(A_pos pos, A_deflist deflist, A_stmtlist stmtlist);
A_stmtlist A_Stmtlist(A_pos pos, A_stmt stmt, A_stmtlist tail);
A_stmt A_Exp_Stmt(A_pos pos, A_exp exp);
A_stmt A_Comp_Stmt(A_pos pos, A_compst compst);
A_stmt A_Ret_Stmt(A_pos pos, A_exp retexp);
A_stmt A_If_Stmt(A_pos pos, A_exp cond, A_stmt if_stmt);
A_stmt A_If_Else_Stmt(A_pos pos, A_exp cond, A_stmt if_stmt, A_stmt else_stmt);
A_stmt A_While_Stmt(A_pos pos, A_exp cond, A_stmt body);
// A.1.6 Local definitions.
A_def A_Def(A_pos pos, A_specifier type, A_declist declist);
A_deflist A_Deflist(A_pos pos, A_def def, A_deflist tail);
A_dec A_Dec(A_pos pos, A_vardec var, A_exp exp);
A_declist A_Declist(A_pos pos, A_dec dec, A_declist tail);
// A.1.7 Expressions.
A_exp A_Bop_Exp(A_pos pos, A_bop op, A_exp left, A_exp right);
A_exp A_Uop_Exp(A_pos pos, A_uop op, A_exp exp);
A_exp A_Call_Exp(A_pos pos, A_id fun, A_args args);
A_exp A_Array_Exp(A_pos pos, A_exp arr, A_exp index);
A_exp A_Struct_Exp(A_pos pos, A_exp record, A_id field);
A_exp A_Id_Exp(A_pos pos, A_id id);
A_exp A_Int_Exp(A_pos pos, int intt);
A_exp A_Float_Exp(A_pos pos, float floatt);
A_args A_Args(A_pos pos, A_exp exp, A_args tail);

extern A_program program_root;
#endif //ABSYNTAX_H_