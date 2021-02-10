#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h> // [M] Já tem math.h pq não usar log (isso vai fazer sentido mais pra frente)

enum commands{
    INSERT  = 'i',
    DELETE  = 'd',
    QUIT    = 'q',
    SAVE    = 'w',
    PRINT   = 'p',
};

typedef struct linha {
    char ch[8192]; // [M] nome mais descritivo, por favor
    size_t linhaSize;
} Linha_t;

typedef struct texto {
    Linha_t linhas[8192];
    size_t totalLinhas;
} Texto_t;
Texto_t texto;

// [M] Nit-pick: portunglês 😡

void assignAllLinhas(FILE *arquivo);
void editExistingFile(char *arquivo);
void getCommand(char **command);
void saveToFile(char *nomeArquivo);

int editLoop(char *nomeArquivo);
int explainProgram();
int printAllFile();
int getIndent();
void insertLinha(int linha);
void printLinha(int linha, char *string);
void deleteLinha(int linha);

int main(int argc, char **argv)
{
    switch (argc) {
        case 1: // [M] O quê é essa opção?
            editLoop("\0"); // [M] Essa string significa "NO_FILE"? Pq nn tacar isso numa constante global?
            break;
        case 2: // [M] O quê é essa opção?
            editExistingFile(argv[1]);
            break;
        default: // [M] O quê é essa(S) opção(ões)?
            explainProgram();
            break;
    }
    return 0;
}

void editExistingFile(char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");


    assignAllLinhas(arquivo);
    fclose(arquivo);

    editLoop(nomeArquivo);
}

void assignAllLinhas(FILE *arquivo)
{
    // [M] Não acho que esse nome explica o quê a função faz
    // [M] Initialize? Clean? WHAT IS GOING ON IN HERE
    Linha_t *linha = texto.linhas;
    int i = texto.totalLinhas = 0;
    while ( fgets((linha+i)->ch, sizeof((linha+i)->ch), arquivo) ) {
        i++;
    }
    texto.totalLinhas = i;
}

int printAllFile()
{
    // [M] 'All' implica múltiplos, use 'Entire' ou omita o adjetivo
    // [M] Isso foi muito nit-pick, desculpa kkkkk
    Linha_t *linha = texto.linhas;
    int i = 0;

    while ( strcmp( (linha+i)->ch, "\0" ) ) {
        printLinha(i+1, (linha+i)->ch);
        i++;
    }
    return 0;
}

// [M] Comments aren't a good source control, source control is a good source control
/* void getCommand(char **command) */
/* { */
/*     char buffer[256]; */
/*     int j=0, ctr=0; */

/*     fgets(buffer, sizeof(buffer), stdin); */
/*     for(int i=0;i<=(strlen(buffer));i++) */
/*     { */
/*         // if space or NULL found, assign NULL into newString[ctr] */
/*         if(buffer[i]==' '||buffer[i]=='\0') */
/*         { */
/*             command[ctr][j]='\0'; */
/*             ctr++;  //for next word */
/*             j=0;    //for next word, init index to 0 */
/*         } */
/*         else */
/*         { */
/*             command[ctr][j]=buffer[i]; */
/*             j++; */
/*         } */
/*     } */
/* } */

int explainProgram()
{
    printf("\
            \nO programa funciona do seguinte jeito:\
            \n\t\t./jed \"arquivo\"\t-> edição de um arquivo de texto.\
            \n\t\t./jed\t\t-> começa a escrever um texto na memória\
            \n\n");
    return 0;
}

void saveToFile(char *nomeArquivo)
{
    // [M] 'dumpToFile'? Sugestão, mas gostei do nome
    
    FILE *arquivo = fopen(nomeArquivo, "w");
    for (int i = 0; i < texto.totalLinhas; i++) {
        fputs(texto.linhas[i].ch, arquivo);
    }
    fclose(arquivo); // [M] Muito obrigado por dar open e close na mesma função kkkkk
}

int editLoop(char *nomeArquivo)
{
    // [M] Função bem grande, imagino que já tentou refatorar; se quiser ajuda só chamar
    char command;
    char buffer[256];
    char aux[256];
    int linha=0;

    char *nomeArq = malloc(256); // [M] Não vejo o free correspondente na função
    strcpy(nomeArq, nomeArquivo);

    while(1) {
        
        // [M] Excelente bloco pra tentar colocar em uma outra função
        strcpy(aux, "\0");
        printf("%ld\n", texto.totalLinhas);
        printf(": ");
        fgets(buffer, sizeof(buffer), stdin);
        system("clear");
        sscanf(buffer, " %c %s", &command, aux);
            sscanf(aux, "%d", &linha);
        
        // [M] Outra função aqui, perhaps?
        switch (command) {
            case INSERT:
                /* comando 'i' */
                insertLinha(linha);
                break;
            case DELETE:
                /* comando 'd' */
                sscanf(aux, "%d", &linha);
                deleteLinha(linha);
                break;
            case PRINT:
                /* comando 'p' */
                if (!strcmp(aux, "%")){
                    printAllFile();
                    break;
                }
                sscanf(aux, "%d", &linha);

                printLinha(linha, texto.linhas[linha-1].ch);
                break;
            case SAVE:
                /* comando 'w' */
                if(!strcmp(aux, "\0")){
                    if(!strcmp(nomeArq, "\0")) {
                        printf("Não há arquivo para salvar\n");
                        break; // [M] Se isso fosse uma função poderiam ser 'return'
                    }
                    saveToFile(nomeArq);
                    break;
                }
                saveToFile(aux);
                break;
            case QUIT:
                /* comando 'q' */
                return 0;
            default:
                printf("Comando inválido\n");
                break;
        }
    }
}

void insertLinha(int linha)
{
    int tL = texto.totalLinhas; // [M] Vc faz essa variável mas não está usando onde poderia wtf
    if (linha > (texto.totalLinhas + 1)) {
        for (int i = 0; i <= linha - tL - 1; i++) {
            strcpy(texto.linhas[tL+i].ch, "\n");
            texto.totalLinhas++;
        }
    } else if (linha == texto.totalLinhas + 1) {
        texto.totalLinhas++;
    }
    fgets(texto.linhas[linha-1].ch, sizeof(texto.linhas[linha-1].ch), stdin);
}

void printLinha(int linha, char *string)
{
    int indent=getIndent();
    printf("%*d| %s", indent, linha, string);
}

int getIndent()
{
    // [M] Isso aqui poderia ser um log base 10 + 1 (but I like it this way)
    int indent=0, totalLinhas;
    totalLinhas = texto.totalLinhas; // [M] pq não deixar a inicialização inline?
    while (totalLinhas != 0) {
        totalLinhas = totalLinhas/10;
        indent++;
    }
    return indent;
}

void deleteLinha(int linha)
{
    if (linha > texto.totalLinhas) {
        return;
    }
    for (int i=linha-1; i < texto.totalLinhas; i++) {
        strcpy(texto.linhas[i].ch, texto.linhas[i+1].ch); // [M] OOF numa lista circular não ia precisar
    }
    strcpy(texto.linhas[texto.totalLinhas-1].ch, "\0");
    texto.totalLinhas--;
}
