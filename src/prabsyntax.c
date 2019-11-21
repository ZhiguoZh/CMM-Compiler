#include <stdarg.h>

#include "../inc/prabsyntax.h"
#include "../inc/symbol.h"

static void print_program(FILE* out, A_program program, int d);
static void print_extdeflist(FILE* out, A_extdeflist extdeflist, int d);
static void print_extdef(FILE* out, A_extdef extdef, int d);
static void print_extdeclist(FILE* out, A_extdeclist extdeclist, int d);
static void print_specifier(FILE* out, A_specifier specifier, int d);
static void print_struct_specifier(FILE* out, A_struct_specifier struct_specifier, int d);
static void print_opttag(FILE* out, A_opt_tag opt_tag, int d);
static void print_tag(FILE* out, A_tag tag, int d);
static void print_vardec(FILE* out, A_vardec vardec, int d);
static void print_fundec(FILE* out, A_fundec fundec, int d);
static void print_varlist(FILE* out, A_varlist varlist, int d);
static void print_paramdec(FILE* out, A_paramdec paramdec, int d);
static void print_compst(FILE* out, A_compst compst, int d);
static void print_stmtlist(FILE* out, A_stmtlist stmtlist, int d);
static void print_stmt(FILE* out, A_stmt stmt, int d);
static void print_def(FILE* out, A_def def, int d);
static void print_deflist(FILE* out, A_deflist deflist, int d);
static void print_dec(FILE* out, A_dec dec, int d);
static void print_declist(FILE* out, A_declist declist, int d);
static void print_exp(FILE* out, A_exp exp, int d);
static void print_args(FILE* out, A_args args, int d);

static void print_with_indent(FILE* out, int indent, const char* format, ...) {
    va_list args;
    va_start(args, format);
    for (int i = 0; i < indent; ++i) {
        fprintf(out, " ");
    }
    vfprintf(out, format, args);
    va_end(args);
}

void print_absyntax(FILE* out, A_program program_root) {
    int indent = 0;
    print_program(out, program_root, indent);
}

static void print_program(FILE* out, A_program program, int d) {
    if (program == NULL) return;
    print_with_indent(out, d, "Program  (%d)\n", program->pos);
    print_extdeflist(out, program->extdeflist, d+2);
}

static void print_extdeflist(FILE* out, A_extdeflist extdeflist, int d) {
    if (extdeflist == NULL) return;
    print_with_indent(out, d, "ExtDefList  (%d)\n", extdeflist->pos);
    print_extdef(out, extdeflist->extdef, d+2);
    print_extdeflist(out, extdeflist->tail, d+2);
}

static void print_extdef(FILE* out, A_extdef extdef, int d) {
    if (extdef == NULL) return;
    print_with_indent(out, d, "ExtDef  (%d)\n", extdef->pos);
    switch (extdef->kind) {
    case A_extdef_var: {
        print_specifier(out, extdef->u.var.type, d+2);
        print_extdeclist(out, extdef->u.var.varlist, d+2);
        print_with_indent(out, d+2, "SEMI\n");
        break;
    }
    case A_extdef_struct: {
        print_specifier(out, extdef->u.struct_type, d+2);
        print_with_indent(out, d+2, "SEMI\n");
        break;
    }
    case A_extdef_func: {
        print_specifier(out, extdef->u.func.ret_type, d+2);
        print_fundec(out, extdef->u.func.fundec, d+2);
        print_compst(out, extdef->u.func.body, d+2);
        break;
    }
    default:
        break;
    }
}

static void print_extdeclist(FILE* out, A_extdeclist extdeclist, int d) {
    if (extdeclist == NULL) return;
    print_with_indent(out, d, "ExtDecList  (%d)\n", extdeclist->pos);
    print_vardec(out, extdeclist->vardec, d+2);
    if (extdeclist->tail != NULL) {
        print_with_indent(out, d+2, "COMMA\n");
        print_extdeclist(out, extdeclist->tail, d+2);
    }
}

static void print_specifier(FILE* out, A_specifier specifier, int d) {
    if (specifier == NULL) return;
    print_with_indent(out, d, "Specifier  (%d)\n", specifier->pos);
    switch (specifier->kind)
    {
    case A_builtin_spec: {
        print_with_indent(out, d+2, specifier->u.builtin == A_int_type ? "Type: int\n": "Type: float\n");
        break;
    }
    case A_struct_spec: {
        print_struct_specifier(out, specifier->u.structt, d+2);
        break;
    }
    default:
        break;
    }
}

static void print_struct_specifier(FILE* out, A_struct_specifier struct_specifier, int d) {
    if (struct_specifier == NULL) return;
    print_with_indent(out, d, "StructSpecifier  (%d)\n", struct_specifier->pos);
    print_with_indent(out, d+2, "STRUCT\n");
    switch (struct_specifier->kind)
    {
    case A_struct_def: {
        print_opttag(out, struct_specifier->u.struct_def.opttag, d+2);
        print_with_indent(out, d+2, "LC\n");
        print_deflist(out, struct_specifier->u.struct_def.deflist, d+2);
        print_with_indent(out, d+2, "RC\n");
        break;
    }
    case A_struct_dec: {
        print_tag(out, struct_specifier->u.tag, d+2);
        break;
    }
    default:
        break;
    }
}

static void print_opttag(FILE* out, A_opt_tag opt_tag, int d) {
    if (opt_tag == NULL) return;
    print_with_indent(out, d, "OptTag  (%d)\n", opt_tag->pos);
    print_with_indent(out, d+2, "ID:  %s\n", symbol_name(opt_tag->id));
}

static void print_tag(FILE* out, A_tag tag, int d) {
    if (tag == NULL) return;
    print_with_indent(out, d, "Tag  (%d)\n", tag->pos);
    print_with_indent(out, d+2, "ID:  %s\n", symbol_name(tag->id));
}

static void print_vardec(FILE* out, A_vardec vardec, int d) {
    if (vardec == NULL) return;
    print_with_indent(out, d, "VarDec  (%d)\n", vardec->pos);
    switch(vardec->kind) {
        case A_id_var: {
            print_with_indent(out, d+2, "ID:  %s\n", symbol_name(vardec->u.id));
            break;
        }
        case A_arr_var: {
            print_vardec(out, vardec->u.arr.vardec, d+2);
            print_with_indent(out, d+2, "LB\n");
            print_with_indent(out, d+2, "INT:  %d\n", vardec->u.arr.len);
            print_with_indent(out, d+2, "RB\n");
            break;
        }
        default:
            break;
    }
}

static void print_fundec(FILE* out, A_fundec fundec, int d) {
    if (fundec == NULL) return;
    print_with_indent(out, d, "FunDec  (%d)\n", fundec->pos);
    print_with_indent(out, d+2, "ID:  %s\n", symbol_name(fundec->name));
    print_with_indent(out, d+2, "LP\n");
    print_varlist(out, fundec->varlist, d+2);
    print_with_indent(out, d+2, "RP\n");
}

static void print_varlist(FILE* out, A_varlist varlist, int d) {
    if (varlist == NULL) return;
    print_with_indent(out, d, "VarList  (%d)\n", varlist->pos);
    print_paramdec(out, varlist->param, d+2);
    print_with_indent(out, d+2, "COMMA\n");
    print_varlist(out, varlist->tail, d+2);
}

static void print_paramdec(FILE* out, A_paramdec paramdec, int d) {
    if (paramdec == NULL) return;
    print_with_indent(out, d, "ParamDec  (%d)\n", paramdec->pos);
    print_specifier(out, paramdec->type, d+2);
    print_vardec(out, paramdec->vardec, d+2);
}

static void print_compst(FILE* out, A_compst compst, int d) {
    if (compst == NULL) return;
    print_with_indent(out, d, "CompSt  (%d)\n", compst->pos);
    print_with_indent(out, d+2, "LC\n");
    print_deflist(out, compst->deflist, d+2);
    print_stmtlist(out, compst->stmtlist, d+2);
    print_with_indent(out, d+2, "RC\n");
}

static void print_stmtlist(FILE* out, A_stmtlist stmtlist, int d) {
    if (stmtlist == NULL) return;
    print_with_indent(out, d, "StmtList  (%d)\n", stmtlist->pos);
    print_stmt(out, stmtlist->stmt, d+2);
    print_stmtlist(out, stmtlist->tail, d+2);
}

static void print_stmt(FILE* out, A_stmt stmt, int d) {
    if (stmt == NULL) return;
    print_with_indent(out, d, "Stmt  (%d)\n", stmt->pos);
    switch (stmt->kind) {
        case A_exp_stmt: {
            print_exp(out, stmt->u.exp, d+2);
            print_with_indent(out, d+2, "SEMI\n");
            break;
        }
        case A_comp_stmt: {
            print_compst(out, stmt->u.compst, d+2);
            break;
        }
        case A_ret_stmt: {
            print_with_indent(out, d+2, "RETURN\n");
            print_exp(out, stmt->u.retExp, d+2);
            print_with_indent(out, d+2, "SEMI\n");
            break;
        }
        case A_if_stmt: {
            print_with_indent(out, d+2, "IF\n");
            print_with_indent(out, d+2, "LP\n");
            print_exp(out, stmt->u.iff.cond, d+2);
            print_with_indent(out, d+2, "RP\n");
            print_stmt(out, stmt->u.iff.if_stmt, d+2);
            break;
        }
        case A_if_else_stmt: {
            print_with_indent(out, d+2, "IF\n");
            print_with_indent(out, d+2, "LP\n");
            print_exp(out, stmt->u.ifelse.cond, d+2);
            print_with_indent(out, d+2, "RP\n");
            print_stmt(out, stmt->u.ifelse.if_stmt, d+2);
            print_with_indent(out, d+2, "ELSE\n");
            print_stmt(out, stmt->u.ifelse.else_stmt, d+2);
            break;
        }
        case A_while_stmt: {
            print_with_indent(out, d+2, "WHILE\n");
            print_with_indent(out, d+2, "LP\n");
            print_exp(out, stmt->u.whilee.cond, d+2);
            print_with_indent(out, d+2, "RP\n");
            print_stmt(out, stmt->u.whilee.body, d+2);
            break;
        }
        default: 
            break;
    }
}

static void print_def(FILE* out, A_def def, int d) {
    if (def == NULL) return;
    print_with_indent(out, d, "Def  (%d)\n", def->pos);
    print_specifier(out, def->type, d+2);
    print_declist(out, def->declist, d+2);
    print_with_indent(out, d+2, "SEMI\n");
}

static void print_deflist(FILE* out, A_deflist deflist, int d) {
    if (deflist == NULL) return;
    print_with_indent(out, d, "DefList  (%d)\n", deflist->pos);
    print_def(out, deflist->def, d+2);
    print_deflist(out, deflist->tail, d+2);
}


static void print_dec(FILE* out, A_dec dec, int d) {
    if (dec == NULL) return;
    print_with_indent(out, d, "Dec  (%d)\n", dec->pos);
    print_vardec(out, dec->var, d+2);
    if (dec->exp != NULL) {
        print_with_indent(out, d+2, "ASSIGNOP\n");
        print_exp(out, dec->exp, d+2);
    }
}

static void print_declist(FILE* out, A_declist declist, int d) {
    if (declist == NULL) return;
    print_with_indent(out, d, "DecList  (%d)\n", declist->pos);
    print_dec(out, declist->dec, d+2);
    if (declist->tail != NULL) { 
        print_with_indent(out, d+2, "COMMA\n");
        print_declist(out, declist->tail, d+2);
    }
}

static void print_exp(FILE* out, A_exp exp, int d) {
    if (exp == NULL) return;
    print_with_indent(out, d, "Exp  (%d)\n", exp->pos);
    switch (exp->kind) {
        case A_bop_exp: {
            print_exp(out, exp->u.bop.left, d+2);
            switch (exp->u.bop.op) {
                case A_assignOp: {
                    print_with_indent(out, d+2, "ASSIGNOP\n");
                    break;
                }
                case A_andOp: {
                    print_with_indent(out, d+2, "AND\n");
                    break;
                }
                case A_orOp: {
                    print_with_indent(out, d+2, "OR\n");
                    break;
                }
                case A_gtOp:
                case A_ltOp:
                case A_geOp:
                case A_leOp:
                case A_eqOp:
                case A_neOp: {
                    print_with_indent(out, d+2, "RELOP\n");
                    break;
                }
                case A_plusOp: {
                    print_with_indent(out, d+2, "PLUS\n");
                    break;
                }
                case A_minusOp: {
                    print_with_indent(out, d+2, "MINUS\n");
                    break;
                }
                case A_starOp: {
                    print_with_indent(out, d+2, "STAR\n");
                    break;
                }
                case A_divOp: {
                    print_with_indent(out, d+2, "DIV\n");
                    break;
                }
                default:
                    break;
            }
            print_exp(out, exp->u.bop.right, d+2);
            break;
        }
        case A_uop_exp: {
            switch (exp->u.uop.op) {
                case A_parOp: {
                    print_with_indent(out, d+2, "LP\n");
                    print_exp(out, exp->u.uop.exp, d+2);
                    print_with_indent(out, d+2, "RP\n");
                    break;
                }
                case A_notOp: {
                    print_with_indent(out, d+2, "NOT\n");
                    print_exp(out, exp->u.uop.exp, d+2);
                    break;
                }
                case A_uminusOp: {
                    print_with_indent(out, d+2, "MINUS\n");
                    print_exp(out, exp->u.uop.exp, d+2);
                    break;
                }
                default:
                    break;
            }
        }
        case A_call_exp: {
            print_with_indent(out, d+2, "ID:  %s\n", symbol_name(exp->u.call.fun));
            print_with_indent(out, d+2, "LP\n");
            print_args(out, exp->u.call.args, d+2);
            print_with_indent(out, d+2, "RP\n");
            break;
        }
        case A_array_exp: {
            print_exp(out, exp->u.array.arr, d+2);
            print_with_indent(out, d+2, "LB\n");
            print_exp(out, exp->u.array.index, d+2);
            print_with_indent(out, d+2, "RB\n");
            break;
        }
        case A_struct_exp: {
            print_exp(out, exp->u.structt.record, d+2);
            print_with_indent(out, d+2, "DOT\n");
            print_with_indent(out, d+2, "ID:  %s\n", symbol_name(exp->u.structt.field));
            break;
        }
        case A_id_exp: {
            print_with_indent(out, d+2, "ID:  %s\n", symbol_name(exp->u.id));
            break;
        }
        case A_int_exp: {
            print_with_indent(out, d+2, "INT:  %d\n", exp->u.intt);
            break;
        }
        case A_float_exp: {
            print_with_indent(out, d+2, "FLOAT:  %f\n", exp->u.floatt);
            break;
        }
        default:
            break;
    }
}

static void print_args(FILE* out, A_args args, int d) {
    if (args == NULL) return;
    print_with_indent(out, d, "Args\n");
    print_exp(out, args->exp, d+2);
    if (args->tail != NULL) {
        print_with_indent(out, d+2, "COMMA\n");
        print_args(out, args->tail, d+2);
    }
}