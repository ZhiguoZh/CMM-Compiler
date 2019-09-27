%option yylineno
%x COMMENT

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../inc/absyntax.h"
    #include "syntax.h"

    int yycolumn = 1;
    #define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn;\
    yylloc.last_column = yycolumn + yyleng - 1;\
    yycolumn += yyleng;
%}
ddigit [0-9]
odigit [0-7]
hdigit [0-9a-fA-F]
letter [_a-zA-Z]
%%
    /*Integer and float.*/
0{odigit}{1,32} { yylval.intt = strtol(yytext, NULL, 8); return INT; } 
{ddigit}{1,32} { yylval.intt = strtol(yytext, NULL, 10); return INT; }
0[xX]{hdigit}{1,32} { yylval.intt = strtol(yytext, NULL, 16); return INT; }  
{ddigit}+\.{ddigit}+|({ddigit}*\.?{ddigit}+|{ddigit}+\.)[Ee][+-]?{ddigit}+ { yylval.floatt = atof(yytext); return FLOAT; }

    /*Space, reserved word and identification.*/
[ \t] { continue; }
\n { yycolumn = 1; continue; }
"int"   { yylval.type = A_int_type; return TYPE; }
"float" { yylval.type = A_float_type; return TYPE; }
"struct" { return STRUCT; }
"return" { return RETURN; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
{letter}({letter}|{ddigit}){0,31} { yylval.id = strdup(yytext); return ID; }

    /*Comments*/
\/\/[^\n]*\n { continue; }
\/\* { BEGIN COMMENT; }
<COMMENT>\*\/ { BEGIN INITIAL; }
<COMMENT>. { continue; }

    /*Operators*/
";" { return SEMI; }
"," { return COMMA; }
"=" { return ASSIGNOP; }
">" { return GT; }
"<" { return LT; }
">=" { return GE; }
"<=" { return LE; }
"==" { return EQ; }
"!=" { return NE; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return STAR; }
"/" { return DIV; }
"&&" { return AND; }
"||" { return OR; }
"." { return DOT; }
"!" { return NOT; }
"(" { return LP; }
")" { return RP; }
"[" { return LB; }
"]" { return RB; }
"{" { return LC; }
"}" { return RC; }
. { printf("Error type A at line %d: Mysterious characters \'%s\'\n", yylineno, yytext); exit(1);}
%%