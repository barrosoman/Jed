#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h> // [M] Já tem math.h pq não usar log (isso vai fazer sentido mais pra frente)

#define NO_FILE ""

enum commands{
    INSERT  = 'i',
    DELETE  = 'd',
    QUIT    = 'q',
    SAVE    = 'w',
    PRINT   = 'p',
    LOCATE  = 'f',
    SUBSTITUTE  = 's',
};

typedef struct line {
    char string[8192];
    size_t lineSize;
} Line_t;

typedef struct text {
    Line_t lines[8192];
    size_t totalLines;
} Texto_t;
Texto_t text;

void loadFileToMemory(FILE *arquivo);
void editExistingFile(char *arquivo);
void saveToFile(char *nomeArquivo);
void orgAuxForSubs(char *aux);
void substituteWord(int line, char *from, char *to);

int locateWord(char *str, char *word);
int editLoop(char *nomeArquivo);
int explainProgram();
int printEntireFile();
int getIndent();

// [TODO] MUDAR (int line) para algo mais intendível, typedef LineINdex por exemplo.
void insertLine(int line);
void printLine(int line);
void deleteLine(int line);

int main(int argc, char **argv)
{
    switch (argc) {
        case 1: // [M] O quê é essa opção?
            editLoop(NO_FILE);
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

    loadFileToMemory(arquivo);
    fclose(arquivo);

    editLoop(nomeArquivo);
}

void loadFileToMemory(FILE *arquivo)
{
    Line_t *line = text.lines;
    int i = text.totalLines = 0;
    while ( fgets((line+i)->string, sizeof((line+i)->string), arquivo) ) {
        i++;
    }
    text.totalLines = i;
}

int printEntireFile()
{
    Line_t *line = text.lines;
    int i = 0;

    while ( strcmp( (line+i)->string, "\0" ) ) {
        printLine(i);
        i++;
    }
    return 0;
}

int explainProgram()
{
    printf("\
            \nO programa funciona do seguinte jeito:\
            \n\t\t./jed \"arquivo\"\t-> edição de um arquivo de text.\
            \n\t\t./jed\t\t-> começa a escrever um text na memória\
            \n\n");
    return 0;
}

void saveToFile(char *nomeArquivo)
{
    // [M] 'dumpToFile'? Sugestão, mas gostei do nome
    FILE *arquivo = fopen(nomeArquivo, "w");
    for (int i = 0; i < text.totalLines; i++) {
        fputs(text.lines[i].string, arquivo);
    }
    fclose(arquivo);
}

int editLoop(char *nomeArquivo) // [TODO] MAYBE PUT ARRAY[256]
{
    char command;
    char buffer[256];
    char aux[256];
    int line=0;

    char *nomeArq = malloc(256); // [TODO] WHY MALLOC?
    strcpy(nomeArq, nomeArquivo);
    int index;

    while(1) {

        // [M] Excelente bloco pra tentar colocar em uma outra função
        strcpy(aux, "\0");
        printf(": ");
        fgets(buffer, sizeof(buffer), stdin);
        system("clear");
        sscanf(buffer, " %c %[^\n]s", &command, aux);
        sscanf(aux, "%d", &line);

        // [M] Outra função aqui, perhaps?
        switch (command) {
            case INSERT:
                /* comando 'i' */
                insertLine(line);
                break;
            case DELETE:
                /* comando 'd' */
                sscanf(aux, "%d", &line);
                deleteLine(line);
                break;
            case PRINT:
                /* comando 'p' */
                if (!strcmp(aux, "%")){
                    printEntireFile();
                    break;
                }
                sscanf(aux, "%d", &line);

                printLine(line-1);
                break;
            case SAVE:
                /* comando 'w' */
                // [TODO] BOTAR NUMA FUNCAO PORRA

                if(!strcmp(aux, NO_FILE)){
                    if(!strcmp(nomeArq, NO_FILE)) {
                        printf("Não há arquivo para salvar\n");
                        break; // [M] Se isso fosse uma função poderiam ser 'return'
                    }
                    saveToFile(nomeArq);
                    break;
                }
                strcpy(nomeArq, aux);
                strcpy(nomeArquivo, aux);
                saveToFile(nomeArq);
                break;
            case QUIT:
                /* comando 'q' */
                free(nomeArq);
                return 0;
            case LOCATE:
                for (int i = 0; i < text.totalLines; i++) {
                    index = locateWord(text.lines[i].string, aux);
                    if (index != -1) {
                        printLine(i);
                    }
                }
                break;
            case SUBSTITUTE:
                /* printf("%s\n", aux); */
                orgAuxForSubs(aux);
                break;
            default:
                printf("Comando inválido\n");
                break;
        }
    }
}

void insertLine(int line) {
    int tL = text.totalLines;
    if (line > (text.totalLines + 1)) {
        for (int i = 0; i <= line - tL - 1; i++) {
            strcpy(text.lines[tL+i].string, "\n");
            text.totalLines++;
        }
    } else if (line == text.totalLines + 1) {
        text.totalLines++;
    }
    fgets(text.lines[line-1].string, sizeof(text.lines[line-1].string), stdin);
}

void printLine(int line)
{
    int indent=getIndent();
    printf("%*d| %s", indent, line+1, text.lines[line].string);
}

int getIndent()
{
    // [M] Isso aqui poderia ser um log base 10 + 1 (but I like it this way)
    int indent=0,
        totalLines = text.totalLines;

    while (totalLines != 0) {
        totalLines = totalLines/10;
        indent++;
    }
    return indent;
}

void deleteLine(int line)
{
    if (line > text.totalLines) {
        return;
    }
    for (int i=line-1; i < text.totalLines; i++) {
        strcpy(text.lines[i].string, text.lines[i+1].string);
    }
    strcpy(text.lines[text.totalLines-1].string, "\0");
    text.totalLines--;
}

int locateWord(char *str, char *word)
{
    int strLen=strlen(str),
           wordLen=strlen(word),
           found;

    for(int i=0; i<strLen - wordLen; i++) {
        found = 1;
        for(int j=0; j<wordLen; j++) {
            if(str[i + j] != word[j]) {
                found = 0;
                break;
            }
        }
        if(found == 1) {
            return i;
        }
    }
    return -1;
}

void orgAuxForSubs(char *aux) {
    char line[8], from[8192], to[8192];
    int index = 0;

    sscanf(aux, "%s %s %s", line, from, to);
    printf("\n--%s--\n--%s--\n--%s--\n", line, from, to);
    if (strcmp(line,"%") == 0) {
        for (int i = 0; i < text.totalLines; i++) {
            index = -1;
            if (strcmp(text.lines[i].string, from) > 0)
            index = locateWord(text.lines[i].string, from);
            if (index != -1 ) {
                substituteWord(i, from, to);
            }
        }
        return;
    }

    substituteWord(atoi(line) - 1, from, to);
}

void substituteWord(int line, char *from, char *to) {
    char before[8192],
         after[8192],
         *string;
    int indexWord, strLen, wordLen;

    string = text.lines[line].string;
    strLen = strlen(string);
    wordLen = strlen(from);

    indexWord = locateWord(string, from);

    strncpy(before, string, indexWord);
    for (int i = (indexWord + wordLen), j = 0;
            i < strLen;
            i++, j++) {
        after[j] = string[i];
    }
    strcpy(string, before);
    strcat(string, to);
    strcat(string, after);
}
