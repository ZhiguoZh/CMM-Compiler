%option yylineno
%x COMMENT

%{
    #include <stdio.h>

    #include "syntax.h"
%}
ddigit [0-9]
odigit [0-7]
hdigit [0-9a-fA-F]
letter [_a-zA-Z]
%%
    /*Integer and float.*/
{ddigit}{1,32}|0{odigit}{1,32}|0[xX]{hdigit}{1,32} { return INT; }
{ddigit}+\.{ddigit}+|({ddigit}*\.?{ddigit}+|{ddigit}+\.)[Ee][+-]?{ddigit}* { return FLOAT; }

    /*Space, reserved word and identification.*/
[ \t\n] { continue; }
"int"|"float" { return TYPE; }
"struct" { return STRUCT; }
"return" { return RETURN; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
{letter}({letter}|{ddigit}){0,31} { return ID; }

    /*Comments*/
\/\/[^\n]*\n { continue; }
\/\* { BEGIN COMMENT; }
<COMMENT>\*\/ { BEGIN INITIAL; }
<COMMENT>. { continue; }

    /*Operators*/
";" { return SEMI; }
"," { return COMMA; }
"=" { return ASSIGNOP; }
">"|"<"|">="|"<="|"=="|"!=" { return RELOP; }
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
. { printf("Error type A at line %d: Mysterious characters \'%s\'\n", yylineno, yytext); }
%%