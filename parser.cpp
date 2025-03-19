#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

// Enum to categorize token types
enum TokenCategory
{
    NUMERIC,
    ARITHMETIC_OPERATOR,
    FILE_END
};

struct LexicalToken
{
    TokenCategory category;
    string value;
};

// Global list to store tokens and a pointer to track current token position.
vector<LexicalToken> lexicalTokens;
int currentTokenIndex = 0;

// Function to retrieve the next token and preview the current token.
LexicalToken getNextToken()
{
    if (currentTokenIndex < lexicalTokens.size())
        return lexicalTokens[currentTokenIndex++];
    return {FILE_END, ""};
}

LexicalToken previewToken()
{
    if (currentTokenIndex < lexicalTokens.size())
        return lexicalTokens[currentTokenIndex];
    return {FILE_END, ""};
}

// Define the Abstract Syntax Tree (AST) node structure.
struct ASTNode
{
    string nodeLabel;
    ASTNode *leftChild;
    ASTNode *rightChild;
    ASTNode(string label) : nodeLabel(label), leftChild(nullptr), rightChild(nullptr) {}
};

// Forward declarations for the recursive parsing functions.
ASTNode *parseExpression();
ASTNode *parseExpressionContinuation(ASTNode *left);
ASTNode *parseTerm();
ASTNode *parseTermContinuation(ASTNode *left);
ASTNode *parseFactor();

// <expression> -> <term> <expression_continuation>
ASTNode *parseExpression()
{
    ASTNode *firstTerm = parseTerm();
    return parseExpressionContinuation(firstTerm);
}

// <expression_continuation> -> + <term> <expression_continuation> | - <term> <expression_continuation> | ε
ASTNode *parseExpressionContinuation(ASTNode *left)
{
    LexicalToken token = previewToken();
    if (token.category == ARITHMETIC_OPERATOR && (token.value == "+" || token.value == "-"))
    {
        getNextToken(); // Consume the operator.
        ASTNode *rightTerm = parseTerm();
        // Create an operation node (either "Add" or "Subtract").
        string operationLabel = (token.value == "+") ? "(Addition)" : "(Subtraction)";
        ASTNode *operationNode = new ASTNode(operationLabel);
        operationNode->leftChild = left;
        operationNode->rightChild = rightTerm;
        // Recurse to maintain left-associativity.
        return parseExpressionContinuation(operationNode);
    }
    return left;
}

// <term> -> <factor> <term_continuation>
ASTNode *parseTerm()
{
    ASTNode *factorNode = parseFactor();
    return parseTermContinuation(factorNode);
}

// <term_continuation> -> * <factor> <term_continuation> | / <factor> <term_continuation> | ε
ASTNode *parseTermContinuation(ASTNode *left)
{
    LexicalToken token = previewToken();
    if (token.category == ARITHMETIC_OPERATOR && (token.value == "*" || token.value == "/"))
    {
        getNextToken(); // Consume the operator.
        ASTNode *rightFactor = parseFactor();
        string operationLabel = (token.value == "*") ? "(Multiplication)" : "(Division)";
        ASTNode *operationNode = new ASTNode(operationLabel);
        operationNode->leftChild = left;
        operationNode->rightChild = rightFactor;
        return parseTermContinuation(operationNode);
    }
    return left;
}

// <factor> -> ( <expression> ) | <integer>
ASTNode *parseFactor()
{
    LexicalToken token = previewToken();
    if (token.category == ARITHMETIC_OPERATOR && token.value == "(")
    {
        getNextToken(); // Consume the opening parenthesis.
        ASTNode *expressionNode = parseExpression();
        LexicalToken nextToken = getNextToken();
        if (nextToken.category != ARITHMETIC_OPERATOR || nextToken.value != ")")
        {
            cerr << "Error: Expected closing parenthesis ')'." << endl;
            exit(1);
        }
        return expressionNode;
    }
    else if (token.category == NUMERIC)
    {
        getNextToken(); // Consume the integer.
        // Create a leaf node for the integer.
        ASTNode *numberNode = new ASTNode(token.value + " (integer)");
        return numberNode;
    }
    else
    {
        cerr << "Error: Unexpected token '" << token.value << "'." << endl;
        exit(1);
    }
}

// Helper function to display the AST structure.
void displayAST(ASTNode *node, int indent = 0)
{
    if (!node)
        return;
    for (int i = 0; i < indent; i++)
        cout << "    ";
    cout << node->nodeLabel << endl;
    displayAST(node->leftChild, indent + 1);
    displayAST(node->rightChild, indent + 1);
}

int main()
{
    // Open the token file generated by the lexer.
    ifstream inputFile("tokens.txt");
    if (!inputFile)
    {
        cerr << "Error: Unable to open 'tokens.txt'." << endl;
        return 1;
    }
    string line;
    while (getline(inputFile, line))
    {
        istringstream tokenStream(line);
        string lexeme, tokenType;
        if (getline(tokenStream, lexeme, ',') && getline(tokenStream, tokenType))
        {
            LexicalToken token;
            token.value = lexeme;
            if (tokenType == "integer")
                token.category = NUMERIC;
            else if (tokenType == "operator")
                token.category = ARITHMETIC_OPERATOR;
            lexicalTokens.push_back(token);
        }
    }
    inputFile.close();

    // Begin parsing the tokens to construct the AST.
    ASTNode *astRoot = parseExpression();

    // Check for any additional tokens left unparsed.
    if (currentTokenIndex < lexicalTokens.size())
    {
        cerr << "Error: Unexpected extra tokens remaining." << endl;
        return 1;
    }

    cout << "Generated Abstract Syntax Tree:" << endl;
    displayAST(astRoot);
    return 0;
}
