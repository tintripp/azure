#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum  {
    IDENTIFIER,
    KEYWORD,
    SEPARATOR,
    OPERATOR,
    LITERAL,
    // COMMENT, this will be ignored
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;
    // optionally, location info for error messages
} Token;

typedef struct {
    const char *source;
    size_t length;
    size_t pos;
    int line;
    int column;
} Lexer;

void Lexer_Init(Lexer *lexer, const char *source, int length){
    lexer->source = source;
    lexer->length = length;

}

bool Lexer_NextToken(Lexer *lexer, Token *out){
    printf("NextToken");
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

char *Lexer_ReadFile(FILE *f){

    char *str = malloc(Lexer_FileSize(f) + 1); // + 1 for nullterm
    int c, i = 0;
    while((c = fgetc(f)) != EOF) {
        str[i++] = c;
    }

    str[i] = '\0';

    return str;
}

int main(int argc, char *argv[]) {
    printf("Azure Lexer\ninput: %s\n",argv[1]);

    Lexer *lexer;
    if (argc > 1) {

        FILE *f = fopen(argv[1], "r");
        if (!f) return NULL;

        Lexer_Init(lexer, Lexer_ReadFile(f), Lexer_FileSize(f));

        fclose(f);
    }

    if (lexer)
        printf("%s\n", lexer->source);
    else
        printf("Null\n");

    return 0;
}