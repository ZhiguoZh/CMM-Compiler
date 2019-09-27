%{
#include <stdio.h>

#include "../inc/absyntax.h"

extern int yylex();
extern int yylineno;
%}

%union {
    A_program program;
    A_extdeflist extdeflist;
    A_extdef extdef;
    A_extdeclist extdeclist;
    A_specifier specifier;
    A_struct_specifier struct_specifier;
    A_opt_tag opttag;
    A_tag tag;
    A_vardec vardec;
    A_fundec fundec;
    A_varlist varlist;
    A_paramdec paramdec;
    A_compst compst;
    A_stmtlist stmtlist;
    A_stmt stmt;
    A_def def;
    A_deflist deflist;
    A_dec dec;
    A_declist declist;
    A_exp exp;
    A_args args;
    A_type type;
    A_id id;
    char* relop;
    int intt;
    float floatt;
}

%token <intt> INT 
%token <floatt> FLOAT
%token <type> TYPE 
%token <id> ID
%token STRUCT RETURN IF ELSE WHILE
%token SEMI COMMA ASSIGNOP LT GT LE GE EQ NE PLUS MINUS STAR DIV AND OR DOT NOT LP RP LB RB LC RC

%right ASSIGNOP
%left OR
%left AND
%left LT GT LE GE EQ NE 
%left PLUS MINUS
%left STAR DIV
%right NOT UMINUS
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <program> Program
%type <extdeflist> ExtDefList
%type <extdef> ExtDef
%type <extdeclist> ExtDecList
%type <specifier> Specifier
%type <struct_specifier> StructSpecifier
%type <opttag> OptTag
%type <tag> Tag
%type <vardec> VarDec
%type <fundec> FunDec
%type <varlist> VarList
%type <paramdec> ParamDec
%type <compst> CompSt
%type <stmtlist> StmtList
%type <stmt> Stmt
%type <deflist> DefList
%type <def> Def
%type <declist> DecList
%type <dec> Dec
%type <exp> Exp
%type <args> Args

%%
/*High-level definitions.*/
Program: ExtDefList {$$ = program_root = A_Program(@1.first_line, $1);}
    ;
ExtDefList: /*empty*/ {$$ = NULL;}
    | ExtDef ExtDefList {$$ = A_Extdeflist(@1.first_line, $1, $2);}
    ;
ExtDef: Specifier ExtDecList SEMI {$$ = A_Extdef_Var(@1.first_line, $1, $2);}
    | Specifier SEMI {$$ = A_Extdef_Struct(@1.first_line, $1);}
    | Specifier FunDec CompSt {$$ = A_Extdef_Func(@1.first_line, $1, $2, $3);}
    ;
ExtDecList: VarDec {$$ = A_Extdeclist(@1.first_line, $1, NULL);}
    | VarDec COMMA ExtDecList {$$ = A_Extdeclist(@1.first_line, $1, $3);}
    ;

/*Specifier*/
Specifier: TYPE {$$ = A_Builtin_Spec(@1.first_line, $1);}
    | StructSpecifier {$$ = A_Struct_Spec(@1.first_line, $1);}
    ;
StructSpecifier: STRUCT OptTag LC DefList RC {$$ = A_Struct_Def(@1.first_line, $2, $4);}
    | STRUCT Tag {$$ = A_Struct_Dec(@1.first_line, $2);}
    ;
OptTag: /*empty*/ {$$ = NULL;}
    | ID {$$ = A_Opt_Tag(@1.first_line, $1);}
    ;
Tag: ID {$$ = A_Tag(@1.first_line, $1);}
    ;
/*Declarators*/
VarDec: ID {$$ = A_Id_Var(@1.first_line, $1);}
    | VarDec LB INT RB {$$ = A_Arr_Var(@1.first_line, $1, $3);}
    ;
FunDec: ID LP VarList RP {$$ = A_Fundec(@1.first_line, $1, $3);}
    | ID LP RP {$$ = A_Fundec(@1.first_line, $1, NULL);}
    ;
VarList: ParamDec COMMA VarList {$$ = A_Varlist(@1.first_line, $1, $3);}
    | ParamDec {$$ = A_Varlist(@1.first_line, $1, NULL);}
    ;
ParamDec: Specifier VarDec {$$ = A_Paramdec(@1.first_line, $1, $2);}
    ;
/*Statements*/
CompSt: LC DefList StmtList RC {$$ = A_Compst(@1.first_line, $2, $3);}
    ;
StmtList: /*empty*/ {$$ = NULL;}
    | Stmt StmtList {$$ = A_Stmtlist(@1.first_line, $1, $2);}
    ;
Stmt: Exp SEMI {$$ = A_Exp_Stmt(@1.first_line, $1);}
    | CompSt {$$ = A_Comp_Stmt(@1.first_line, $1);}
    | RETURN Exp SEMI {$$ = A_Ret_Stmt(@1.first_line, $2);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = A_If_Stmt(@1.first_line, $3, $5);}
    | IF LP Exp RP Stmt ELSE Stmt {$$ = A_If_Else_Stmt(@1.first_line, $3, $5, $7);}
    | WHILE LP Exp RP Stmt {$$ = A_While_Stmt(@1.first_line, $3, $5);}
    ;
/*Local definitions*/
DefList: /*empty*/ {$$ = NULL;}
    | Def DefList {$$ = A_Deflist(@1.first_line, $1, $2);}
    ;
Def: Specifier DecList SEMI {$$ = A_Def(@1.first_line, $1, $2);}
    ;
DecList: Dec {$$ = A_Declist(@1.first_line, $1, NULL);}
    | Dec COMMA DecList {$$ = A_Declist(@1.first_line, $1, $3);}
    ;
Dec: VarDec {$$ = A_Dec(@1.first_line, $1, NULL);}
    | VarDec ASSIGNOP Exp {$$ = A_Dec(@1.first_line, $1, $3);}
    ;
/*Expressions*/
Exp: Exp ASSIGNOP Exp {$$ = A_Bop_Exp(@1.first_line, A_assignOp, $1, $3);}
    | Exp AND Exp {$$ = A_Bop_Exp(@1.first_line, A_andOp, $1, $3);}
    | Exp OR Exp {$$ = A_Bop_Exp(@1.first_line, A_orOp, $1, $3);}
    | Exp GT Exp {$$ = A_Bop_Exp(@1.first_line, A_gtOp, $1, $3);}
    | Exp GE Exp {$$ = A_Bop_Exp(@1.first_line, A_geOp, $1, $3);}
    | Exp LT Exp {$$ = A_Bop_Exp(@1.first_line, A_ltOp, $1, $3);}
    | Exp LE Exp {$$ = A_Bop_Exp(@1.first_line, A_leOp, $1, $3);}
    | Exp EQ Exp {$$ = A_Bop_Exp(@1.first_line, A_eqOp, $1, $3);}
    | Exp NE Exp {$$ = A_Bop_Exp(@1.first_line, A_neOp, $1, $3);}
    | Exp PLUS Exp {$$ = A_Bop_Exp(@1.first_line, A_plusOp, $1, $3);}
    | Exp MINUS Exp {$$ = A_Bop_Exp(@1.first_line, A_minusOp, $1, $3);}
    | Exp STAR Exp {$$ = A_Bop_Exp(@1.first_line, A_starOp, $1, $3);}
    | Exp DIV Exp {$$ = A_Bop_Exp(@1.first_line, A_divOp, $1, $3);}
    | LP Exp RP {$$ = A_Uop_Exp(@1.first_line, A_parOp, $2);}
    | MINUS Exp %prec UMINUS {$$ = A_Uop_Exp(@1.first_line, A_uminusOp, $2);}
    | NOT Exp {$$ = A_Uop_Exp(@1.first_line, A_notOp, $2);}
    | ID LP Args RP {$$ = A_Call_Exp(@1.first_line, $1, $3);}
    | ID LP RP {$$ = A_Call_Exp(@1.first_line, $1, NULL);}
    | Exp LB Exp RB {$$ = A_Array_Exp(@1.first_line, $1, $3);}
    | Exp DOT ID {$$ = A_Struct_Exp(@1.first_line, $1, $3);}
    | ID {$$ = A_Id_Exp(@1.first_line, $1);}
    | INT {$$ = A_Int_Exp(@1.first_line, $1);}
    | FLOAT {$$ = A_Float_Exp(@1.first_line, $1);}
    ;
Args: Exp COMMA Args {$$ = A_Args(@1.first_line, $1, $3);}
    | Exp {$$ = A_Args(@1.first_line, $1, NULL);}
    ;
%%
void yyerror(char* msg) {
   fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg); 
}