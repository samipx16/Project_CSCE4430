%
{
#include <stdio.h>
    FILE *outputFile;
    %
}

% %

        [0 - 9] +
{
    fprintf(outputFile, "%s,integer\n", yytext);
}
"(" { fprintf(outputFile, "%s,operator\n", yytext); }
")" { fprintf(outputFile, "%s,operator\n", yytext); }
"+" { fprintf(outputFile, "%s,operator\n", yytext); }
"-" { fprintf(outputFile, "%s,operator\n", yytext); }
"*" { fprintf(outputFile, "%s,operator\n", yytext); }
"/" { fprintf(outputFile, "%s,operator\n", yytext); }
[ \t\n] + {/* Skip over whitespace characters */}
              . { printf("Encountered an unknown character: %s\n", yytext); }

% %

    int main(int argc, char **argv)
{
    /* Open file to output recognized tokens */
    outputFile = fopen("tokens.txt", "w");
    if (!outputFile)
    {
        perror("Failed to open file");
        return 1;
    }
    yylex();
    fclose(outputFile);
    return 0;
}

int yywrap()
{
    return 1;
}
