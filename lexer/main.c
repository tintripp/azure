#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum  {
    WHITESPACE,
    IDENTIFIER,
    LITERAL,
    OPERATOR,
    KEYWORD,
    SEPARATOR,
    // COMMENT, this will be ignored
} TokenType;

typedef struct {
    TokenType type;

    size_t lexcap;
    char *lexeme;

    // optionally, location info for error messages
} Token;

void Token_Init(Token *token, int lexcap){
    token->type = WHITESPACE;

    token->lexcap = lexcap;
    token->lexeme = calloc(token->lexcap, sizeof(char));
}

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
    printf("NextToken\n");

    if (lexer->pos >= lexer->length) {
        printf("Finished");
        return false;
    }
    
    int i = 0; // for lexeme
    while(
        lexer->pos < lexer->length &&
        lexer->source[lexer->pos] != ' '
    ){
        if (i >= token->lexcap){
            token->lexcap *= 2;

            char *new = realloc(token->lexeme, token->lexcap);
            if (!new){
                free(token->lexeme);
                exit(1);
            }

            token->lexeme = new;
        }

        token->lexeme[i++] = lexer->source[lexer->pos++];
    }
    token->lexeme[i] = '\0';

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
        Token_Init(&t, 10);
        
        Lexer_NextToken(&lexer, &t);
        printf("(%i, %s)\n", t.type, t.lexeme);
        
        /*while (Lexer_NextToken(&lexer, &t)){
            printf("(%i, %s)\n", t.type, t.lexeme);
        }*/
    }


    return 0;
}