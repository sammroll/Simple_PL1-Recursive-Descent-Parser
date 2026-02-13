#include <stdio.h>  //for c I/o
#include <stdlib.h> // for exit()
#include <ctype.h>  // for isalpha(), isalnum(), ...
enum tokenType currentToken;
enum tokenType
{
    READ,
    WRITE,
    ID,
    NUMBER,
    LPAREN,
    RPAREN,
    SEMICOLON,
    COMMA,
    ASSIGN,
    PLUS,
    MINUS,
    TIMES,
    DIV,
    SCAN_EOF
};
char* mnemonic[] = { "READ", "WRITE", "ID", "NUMBER", "LPAREN", "RPAREN", "SEMICOLON", "COMMA",
                    "ASSIGN", "PLUS", "MINUS", "TIMES", "DIV", "SCAN_EOF" };
void lexical_error(char ch)
{
    fprintf(stderr, "Lexical Error. Unexpected character: %c.\n", ch);
}
char lexeme[256] = { '\0' };
unsigned int lexLen = 0;
FILE* src;
unsigned numErrs;
int count = 0;
enum tokenType scan()
{
    static int currentCh = ' ';
    static int tempCh = ' ';
    char* reserved[2] = { "read", "write" };
    lexLen = 0; // clear lexeme buffer for each scan
    lexeme[0] = '\0';
    extern FILE* src; // pointer to FILE handle that binds to source file.
    if (feof(src))
    {
        return SCAN_EOF;
    }
    while ((currentCh = fgetc(src)) != EOF)
    {
        if (isspace(currentCh))
        {
            continue;
        }
        else if (isalpha(currentCh) || currentCh == '_')
        {
            lexeme[0] = currentCh;
            lexLen = 1;

            // build identifier lexeme
            for (tempCh = fgetc(src); isalnum(tempCh) || tempCh == '_';)
            {
                // adds character to lexeme where lexLen is the current length of the lexeme
                lexeme[lexLen++] = tempCh;
                // get next character
                tempCh = fgetc(src);
            }
            lexeme[lexLen] = '\0'; // null terminate lexeme so that it knows its the end of the lexeme
            ungetc(tempCh, src); // put back character that is not a alpha/digit or ‘_’
            // see if lexeme is a reserved word, if not, return ID.
            if (strcmp(lexeme, reserved[0]) == 0)
            {
                return READ;
            }
            else if (strcmp(lexeme, reserved[1]) == 0)
            {
                return WRITE;
            }
            else
            {
                return ID;
            }
        }
        else if (isdigit(currentCh))
        {
            lexeme[0] = currentCh;
            lexLen = 1;
            // reads the numbers
            while (isdigit(tempCh = fgetc(src))) {
                lexeme[lexLen++] = tempCh;
            }

            //check if it could possibly be an ID that starts with a numer like 10x_5y
            if (isalpha(tempCh) || tempCh == '_') {
                lexeme[lexLen++] = tempCh;
                while (isalnum(tempCh = fgetc(src)) || tempCh == '_') {
                    lexeme[lexLen++] = tempCh;
                }
                lexeme[lexLen] = '\0';
                ungetc(tempCh, src);
                return ID;
            }

            // if just pure numbers, then return NUMBER
            lexeme[lexLen] = '\0';
            ungetc(tempCh, src);
            return NUMBER;
        }
        else if (currentCh == '+')
        {
            return PLUS;
        }
        else if (currentCh == '-')
        {
            return MINUS;
        }
        else if (currentCh == '*')
        {
            return TIMES;
        }
        else if (currentCh == '/')
        {
            return DIV;
        }
        else if (currentCh == ';')
        {
            return SEMICOLON;
        }
        else if (currentCh == ',')
        {
            return COMMA;
        }
        else if (currentCh == '(')
        {
            return LPAREN;
        }
        else if (currentCh == ')')
        {
            return RPAREN;
        }
        else if (currentCh == ':')
        {
            // verify if next character is '='
            tempCh = fgetc(src);
            if (tempCh == '=')
            {
                return ASSIGN;
            }
            else
            {
                lexical_error(currentCh);
            }
        }
        else
        {
            lexical_error(currentCh);
        }
    }
    return SCAN_EOF;
}
void parse_error(char* errMsg, char* lexeme) {
    extern unsigned numErrs; //for future if error recovery used
    numErrs++;
    fprintf(stderr, "%s: %s\n", errMsg, lexeme);
}
void match(enum tokenType expected) {
    if (currentToken == expected) {
        currentToken = scan();
    }
    else {
        parse_error("Expected symbol", mnemonic[expected]);
        exit(1);
    }
}
void term();
void factor();
void expr();
void stmt();
void parse();

void expr() {
    term();
	// continue calling term() while currentToken is PLUS or MINUS
    while (currentToken == PLUS || currentToken == MINUS) {
        if (currentToken == PLUS) {
            match(PLUS);
        }
        else {
            match(MINUS);
        }
        term();
    }
}

void term() {
    factor();
    // continue calling factor() while currentToken is TIMES or DIV
    while (currentToken == TIMES || currentToken == DIV) {
        if (currentToken == TIMES) {
            match(TIMES);
        }
        else {
            match(DIV);
        }
        factor();
    }
}

void factor() {
    // check if currentToken is ID, NUMBER, LPAREN
    if (currentToken == ID) {
        match(ID);
    }
    else if (currentToken == NUMBER) {
        match(NUMBER);
    }
    else if (currentToken == LPAREN) {
        match(LPAREN);
        expr();
        match(RPAREN);
    }
    else {
        parse_error("Error in expression: Expected ID, NUMBER, or '('.", lexeme);
    }
}

void stmt() {
    // check if currentToken is ID, READ, WRITE
    if (currentToken == ID) {
        match(ID);
        match(ASSIGN);
        expr();
        match(SEMICOLON);
    }
    else if (currentToken == READ) {
        match(READ);
        match(LPAREN);
        match(ID);
        // continue reading ID's after the COMMA 
        while (currentToken == COMMA) {
            match(COMMA);
            match(ID);
        }
        match(RPAREN);
        match(SEMICOLON);
    }
    else if (currentToken == WRITE) {
        match(WRITE);
        match(LPAREN);
        expr();
        // continue reading ID's after the COMMA 
        while (currentToken == COMMA) {
            match(COMMA);
            expr();
        }
        match(RPAREN);
        match(SEMICOLON);
    }
    else {
        parse_error("Error in statement: Expected ID, READ, or WRITE.", lexeme);
    }
}

void parse() {
    // call scan() one time to begin scanning
	currentToken = scan();
    // continue parsing until the end of file is reached
    while (currentToken != SCAN_EOF)
    {
        stmt();
    }
}

int main(int argc, char* argv[])
{
    extern FILE* src;
    if (argc > 1)
    {
        if (fopen_s(&src, argv[1], "r"))
        {
            fprintf(stderr, "Error opening source file: %s ", argv[1]);
            exit(1);
        }
    }

    parse();
    // if no errors encountered, then parsing of whole file complete
    if (numErrs == 0) {
        printf("\nParsing complete. No errors.\n");
    }

    return 0;

} 
