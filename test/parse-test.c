#include <stdio.h>

#include "../inc/absyntax.h"
#include "../inc/prabsyntax.h"

extern int yyparse(void);

int main() {
    yyparse();
    printf("%p\n", program_root);
    print_absyntax(stdout, program_root);
}