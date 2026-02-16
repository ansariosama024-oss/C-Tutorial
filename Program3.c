#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Define token types
typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_UNKNOWN,
    TOKEN_END
} TokenType;

// Define a structure for tokens
typedef struct {
    TokenType type;
    char value[64];
} Token;

// Function to print token type as a string
const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_END: return "END";
        default: return "UNKNOWN";
    }
}

// Function to get the next token from the input stream
Token getNextToken(const char** input) {
    Token token;
    token.value[0] = '\0';

    // Skip whitespace
    while (isspace(**input)) {
        (*input)++;
    }

    // Check for end of input
    if (**input == '\0') {
        token.type = TOKEN_END;
        return token;
    }

    // Check for specific tokens
    if (isdigit(**input)) {
        const char* start = *input;
        while (isdigit(**input)) {
            (*input)++;
        }
        strncpy(token.value, start, *input - start);
        token.value[*input - start] = '\0';
        token.type = TOKEN_NUMBER;
    }
    else if (strchr("+-*/", **input)) {
        token.type = TOKEN_OPERATOR;
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
    }
    else if (**input == '(') {
        token.type = TOKEN_LPAREN;
        strncpy(token.value, "(", sizeof(token.value));
        (*input)++;
    }
    else if (**input == ')') {
        token.type = TOKEN_RPAREN;
        strncpy(token.value, ")", sizeof(token.value));
        (*input)++;
    }
    else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = **input;
        token.value[1] = '\0';
        (*input)++;
    }

    return token;
}

int main() {
    const char* code = "3 + 5 * (2 - 8)";
    const char* input = code;
    Token token;

    printf("Token Type      Value\n");
    printf("-----------     -----\n");

    while ((token = getNextToken(&input)).type != TOKEN_END) {
        printf("%-15s %-5s\n",
               tokenTypeToString(token.type),
               token.value);
    }

    return 0;
}
