#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef enum  {
    IDENTIFIER,
    LITERAL,
    OPERATOR,
    KEYWORD,
    SEPARATOR,
    // COMMENT, this will be ignored
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;

    // optionally, location info for error messages
} Token;

typedef struct {
    const char *filename;
    const char *source;
    size_t length;
    size_t pos;
    int line;
    int column;
} Lexer;

void Lexer_Init(
    Lexer *lexer, const char *filename, const char *source, size_t length
){
    lexer->filename = filename;
    lexer->source = source;
    lexer->length = length;
    lexer->pos = 0;
}

bool Lexer_NextToken(Lexer *lexer, Token *token){
    unsigned char c;

    char operators[] = {'=', '+', '-'}; // whoops make these strings too?
    char *keywords[] = {
        "int", "char", "for", "if", "while", 
    };
    
    // skip all whitespace
    while (
        (c = (unsigned char)lexer->source[lexer->pos]) &&
        isspace(c)
    ) 
        lexer->pos++;

    int start = lexer->pos;

    if(c == ';'){
        lexer->pos++;
        token->type = SEPARATOR;
    }
    
    else if (isdigit(c)){ // Eat until Not Digit(TM)
        
        token->type = LITERAL; // NUMBER literal?

        while (
            (c = (unsigned char)lexer->source[lexer->pos]) != '\0' && isdigit(c) 
        ) 
            lexer->pos++;
        
    }

    else if (strchr(operators, c) != NULL){ // char IN operators
        
        token->type = OPERATOR;

        while (
            (c = (unsigned char)lexer->source[lexer->pos]) != '\0' && strchr(operators, c) != NULL
        ) 
            lexer->pos++;

    }

    else{ // Eat until Whitespace
        token->type = IDENTIFIER;

        while (
            (c = (unsigned char)lexer->source[lexer->pos]) != '\0' && !isspace(c) 
        ) 
            lexer->pos++;

    }
    
    int length = lexer->pos - start;
    printf("Before RETURN early, length: %i, pos: %li, start: %i\n", length, lexer->pos, start);
    if (!length)
        return false;
    
    // fill lexeme
    token->lexeme = malloc(length + 1);
    for (int i = 0; i < length; i++)
        token->lexeme[i] = lexer->source[start + i];
    token->lexeme[length] = '\0';

    // ok now see if its actually a keyword
    if (token->type == IDENTIFIER){
        for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++){
            //printf("%s\n", keywords[i]);
            if (strcmp(token->lexeme, keywords[i]) == 0)
                token->type = KEYWORD;
        }
    }

    return true; // DO repeat
    // if no more, return false
}

size_t Lexer_FileSize(FILE *f){
    if (fseek(f, 0, SEEK_END) != 0)
        return (size_t)-1;
    
    long size = ftell(f);
    if (size < 0)
        return (size_t)-1;
    
    rewind(f);

    return (size_t)size;
}

char *Lexer_ReadFile(const char *name) {
    FILE *f = fopen(name, "rb");
    if (!f) return NULL;

    size_t size = Lexer_FileSize(f);
    if (size == (size_t)-1) { fclose(f); return NULL; }

    char *buffer = malloc(size + 1); // +1 for null terminator
    if (!buffer) { fclose(f); return NULL; }

    size_t read = fread(buffer, 1, size, f);
    if (read != size) {
        free(buffer);
        fclose(f);
        return NULL;
    }

    buffer[size] = '\0'; // null terminate
    fclose(f);
    return buffer;
}

int main(int argc, char *argv[]) {
    printf("Azure Lexer\ninput: \n\t%s\n",argv[1]);

    Lexer lexer;
    if (argc > 1) {

        const char *source = Lexer_ReadFile(argv[1]);
        Lexer_Init(&lexer, argv[1], source, strlen(source));

        printf("source: \n\t%s\n", lexer.source);

        
        Token t;

        while(Lexer_NextToken(&lexer, &t))
            printf("(%i, %s)\n", t.type, t.lexeme);
            
    }


    return 0;
}