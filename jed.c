#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum commands{
    INSERT  = 'i',
    DELETE  = 'd',
    QUIT    = 'q',
    SAVE    = 'w',
    PRINT   = 'p',
};

typedef struct linha {
    char ch[8192];
    size_t linhaSize;
} Linha_t;

typedef struct texto {
    Linha_t linhas[8192];
    size_t totalLines;
} Texto_t;
Texto_t texto;

int printAllFile();
void assignAllLines(FILE *arquivo);
void editExistingFile(char *arquivo);
int explainProgram();
void getCommand(char **command);

int main(int argc, char **argv) {
    /* char command[20][8192]; */

    switch (argc) {
        case 1:
            printf(": ");
            break;
        case 2:
            editExistingFile(argv[1]);
            break;
        default:
            explainProgram();
            break;
    }

    /* getCommand(); */

    /* int word = 0; */
    /* while (strcmp(command[word], "\n")) { */
    /*     printf("%s\n", command[word]); */
    /*     word++; */
    /* } */

}

void editExistingFile(char *nomeArquivo) {
    system("clear");
    FILE *arquivo = fopen(nomeArquivo, "r");
    char command;
    char buffer[256];
    char aux[256];
    int line=0;


    assignAllLines(arquivo);
    printAllFile();
    while(1) {
        printf(": ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, " %c %s", &command, aux);
        switch (command) {
            case INSERT:
                sscanf(aux, "%d", &line);
                fgets(texto.linhas[line-1].ch, sizeof(texto.linhas[line-1].ch), stdin);
                break;
            case DELETE:
                break;
            case SAVE:
                break;
            case PRINT:
                printAllFile();
                break;
            case QUIT:
                return;
            default:
                printf("Comando inválido\n");
                break;
        }
    }
    fclose(arquivo);
}

void assignAllLines(FILE *arquivo) {
    Linha_t *linha = texto.linhas;
    int i = texto.totalLines = 0;
    while ( fgets((linha+i)->ch, sizeof((linha+i)->ch), arquivo) ) {
        i++;
    }
    texto.totalLines = i;
}

int printAllFile() {
    Linha_t *linha = texto.linhas;
    int i = 0;

    while ( strcmp( (linha+i)->ch, "\0" )) {
        printf("%s", (linha+i)->ch);
        i++;
    }
    return 0;
}

void getCommand(char **command) {
    /* char buffer[256]; */
    /* int j=0, ctr=0; */

    /* fgets(buffer, sizeof(buffer), stdin); */
    /* for(int i=0;i<=(strlen(buffer));i++) */
    /* { */
    /*     // if space or NULL found, assign NULL into newString[ctr] */
    /*     if(buffer[i]==' '||buffer[i]=='\0') */
    /*     { */
    /*         command[ctr][j]='\0'; */
    /*         ctr++;  //for next word */
    /*         j=0;    //for next word, init index to 0 */
    /*     } */
    /*     else */
    /*     { */
    /*         command[ctr][j]=buffer[i]; */
    /*         j++; */
    /*     } */
    /* } */
}

int explainProgram() {
    printf("\
            \nO programa funciona do seguinte jeito:\
            \n\t\t./jed \"arquivo\"\t-> edição de um arquivo de texto.\
            \n\t\t\tou\
            \n\t\t./jed\t\t-> começa a escrever um texto na memória\
            \n\n");
    return 0;
}
