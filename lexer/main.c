#include <stdio.h>
#include <stdlib.h>

char *OpenFile(char *name){
    FILE *f = fopen(name, "r");
    if (!f) return NULL;

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    long size = ftell(f);
    if (size < 0) {
        fclose(f);
        return NULL;
    }
    rewind(f);

    char *str = malloc((size_t)size + 1); // + 1 for nullterm
    int c, i = 0;
    while((c = fgetc(f)) != EOF) {
        str[i++] = c;
    }

    str[i] = '\0';
    fclose(f); 

    return str;
}

int main(int argc, char *argv[]) {
    printf("Hello world!\n%s %s\n",argv[0],argv[1]);

    char *result;
    if (argc > 1)
        result = OpenFile(argv[1]);

    if (result)
        printf("%s\n", result);
    else
        printf("Null\n");

    return 0;
}