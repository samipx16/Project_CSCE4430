Team: Team 7

Team Member: Samip Chalise | 

Team Member: Priyash Jung Shah | 11438731

Term Project 1: “Let’s build our own simple programming language”


Arithmetic Expression Parser
This project implements a simple arithmetic expression parser using Flex (Lexical Analyzer) and C++ (Recursive Descent Parser). The program takes mathematical expressions as input, tokenizes them, and builds an Abstract Syntax Tree (AST).
How to Run:

Step 1:First, Run the following commands to generate and compile the lexer:
flex lexer.l  
gcc lex.yy.c -o lexer

Step 2: Second, Create a file called input.txt and enter a mathematical expression, such as:
57*2-(3+4)

Step 3: Third, Run the lexer to process input.txt: 
lexer.exe < input.txt

and it generates tokens.txt which will be used by the parser later on

Step 4: Then, Compile the recursive descent parser using:
g++ -std=c++11 parser.cpp -o parser

Step 5: After that, Execute the parser to generate the Abstract Syntax Tree (AST):
parser.exe

Finally, For the input like this: 57*2-(3+4), the output will be:
Abstract Syntax Tree:
Subtract
    Multiply
        57 (int)
        2 (int)
    Add
        3 (int)
        4 (int)


