%{
#include <stdio.h>
FILE *output;
%}

%%

[0-9]+    { fprintf(output, "%s,integer\n", yytext); }
"("       { fprintf(output, "%s,operator\n", yytext); }
")"       { fprintf(output, "%s,operator\n", yytext); }
"+"       { fprintf(output, "%s,operator\n", yytext); }
"-"       { fprintf(output, "%s,operator\n", yytext); }
"*"       { fprintf(output, "%s,operator\n", yytext); }
"/"       { fprintf(output, "%s,operator\n", yytext); }
[ \t\n]+  { /* Ignore whitespace */ }
.         { printf("Unknown character: %s\n", yytext); }

%%

int main(int argc, char **argv)
{
    /* Open output file to write tokens */
    output = fopen("tokens.txt", "w");
    if (!output) {
        perror("fopen");
        return 1;
    }
    yylex();
    fclose(output);
    return 0;
}
int yywrap() {
    return 1;
}
