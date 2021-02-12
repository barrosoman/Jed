#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* DEFINITIONS FOR BETTER CODE READABILITY */
#define NO_FILE ""
#define NOT_FOUND -1
typedef unsigned Index;

enum commands{
    INSERT      = 'i',
    DELETE      = 'd',
    QUIT        = 'q',
    SAVE        = 'w',
    PRINT       = 'p',
    LOCATE      = 'f',
    SUBSTITUTE  = 's',
};

/* STRUCTS */
typedef struct line {
    char string[8192];
    size_t lineSize;
} Line_t;

typedef struct text {
    Line_t lines[8192];
    size_t totalLines;
} Texto_t;
Texto_t text;

/* FUNCTIONS */
void loadFileToMemory(FILE *arquivo);
void editExistingFile(char *arquivo);
void saveToFile(char *nomeArquivo);
void organizeAuxForSubstitution(char *aux);
void editLoop(char *nomeArquivo);
void substituteFirstWordOcurrence(Index line, char *from, char *to);
void insertLine(Index line);
void printLine(Index line);
void deleteLine(Index line);
void orgInput(char *command, char *aux, Index *line);
void saveCmd(char *aux, char *nomeArq);
void locateCmd(char *aux, Index line);
void printCmd(char *aux, Index line);
void determineAction(int qtdArguments, char **arguments);
int doCommand(char command, char *aux, char *nomeArq, Index line);
int explainProgram();
int printEntireFile();
int getIndent();
int locateWord(char *str, char *word);
int isEqual(char *string1, char *string2);

/************ MAIN FUNCTION ************/
int main(int argc, char **argv) {
    determineAction(argc, argv);
    return 0;
}

/************ INIT FUNCTIONS ************/
/* Decide o que fazer a partir da quantidade de parametros do programa */
void determineAction(int qtdArguments, char **arguments) {
    switch (qtdArguments) {
        /* ./jed */
        case 1:
            editLoop(NO_FILE);
            break;
        /* ./jed arquivo.txt */
        case 2:
            editExistingFile(arguments[1]);
            break;
        default:
            explainProgram();
            break;
    }
}

/* Explica para o usuário como usar o programa */
int explainProgram() {
    printf("\
            \nO programa funciona do seguinte jeito:\
            \n\t\t./jed \"arquivo\"\t-> edição de um arquivo de text.\
            \n\t\t./jed\t\t-> começa a escrever um text na memória\
            \n\n");
    return 0;
}

/************ READ FILE FUNCTIONS ************/
/* Abre o arquivo para leitura para obtenção das linhas */
void editExistingFile(char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");

    /* Se o arquivo não existir, crie-o */
    if (arquivo == NULL) {
        arquivo = fopen(nomeArquivo, "w");
        fclose(arquivo);
        arquivo = fopen(nomeArquivo, "r");
    }

    loadFileToMemory(arquivo);
    fclose(arquivo);

    editLoop(nomeArquivo);
}

/* Insere as linhas do texto na memória para edição */
void loadFileToMemory(FILE *arquivo) {
    Line_t *line = text.lines;
    int i = text.totalLines = 0;
    while (fgets((line+i)->string, sizeof((line+i)->string), arquivo)) {
        i++;
    }
    text.totalLines = i;
}

/************ MENU FUNCTIONS ************/
/* Função genérica para edição de um arquivo */
void editLoop(char *nomeArquivo) {
    char command = '\0',
         aux[8192] = {},
         nomeArq[512] = {};
    Index line=0;

    strcpy(nomeArq, nomeArquivo);

    while(1) {
        orgInput(&command, aux, &line);
        if (doCommand(command, aux, nomeArq, line) == 1) {
            return;
        }
    }
}

/* Organiza a entrada do usuário e divide em informações
   úteis (comando, linha de execução do comando e parametros) */
void orgInput(char *command, char *aux, Index *line) {
    char buffer[8192] = {};

    printf(": ");
    fgets(buffer, sizeof(buffer), stdin);
    system("clear");
    sscanf(buffer, " %c %[^\n]s", command, aux);
    sscanf(aux, "%d", line);
}

/* Decide que ação fazer a partir do que o usuário escreveu */
int doCommand(char command, char *aux, char *nomeArq, Index line) {
    switch (command) {
        /* comando 'i' */
        case INSERT:
            insertLine(line);
            break;
        /* comando 'd' */
        case DELETE:
            deleteLine(line);
            break;
        /* comando 'p' */
        case PRINT:
            printCmd(aux, line);
            break;
        /* comando 'w' */
        case SAVE:
            saveCmd(aux, nomeArq);
            break;
        /* comando 'l' */
        case LOCATE:
            locateCmd(aux, line);
            break;
        /* comando 's' */
        case SUBSTITUTE:
            organizeAuxForSubstitution(aux);
            break;
        /* comando 'q' */
        case QUIT:
            return 1;
        default:
            printf("Comando inválido\n");
            break;
    }
    return 0;
}

/************ COMMANDS ************/
/****** SAVE COMMAND ******/
/* Função base para o comando de salvar o texto */
void saveCmd(char *aux, char *nomeArq) {
    if(isEqual(aux, NO_FILE)) {
        if(isEqual(nomeArq, NO_FILE)) {
            printf("Não há arquivo para salvar\n");
            return;
        }
        saveToFile(nomeArq);
        return;
    }
    strcpy(nomeArq, aux);
    saveToFile(nomeArq);
}

/* Insere as linhas do texto no arquivo */
void saveToFile(char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    for (int i = 0; i < text.totalLines; i++) {
        fputs(text.lines[i].string, arquivo);
    }
    fclose(arquivo);
}

/****** INSERT COMMAND ******/
/* Pega informação do usuário e insere em uma linha do texto */
void insertLine(Index line) {
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

/****** PRINT COMMAND ******/
/* Função base para o comando de impressão de linha,
   decide se imprime uma ou todas linhas */
void printCmd(char *aux, Index line) {
    if (isEqual(aux, "%")){
        printEntireFile();
        return;
    }
    printLine(line-1);
}

/* Roda um loop para imprimir cada linha */
int printEntireFile() {
    for (int i = 0; i < text.totalLines; i++) {
        printLine(i);
    }
    return 0;
}

/* Imprime uma linha para o stdout */
void printLine(Index line) {
    int indent=getIndent();
    printf("%*d| %s", indent, line+1, text.lines[line].string);
}

/* Calcula quantos espaços dar para a numeração da linha ficar corretamente
   indentada */
int getIndent() {
    int indent=0,
        totalLines = text.totalLines;

    while (totalLines != 0) {
        totalLines = totalLines/10;
        indent++;
    }
    return indent;
}

/****** SUBSTITUTE COMMAND ******/
/* Função base do comando substituir
   Substitui em uma linha ou em todo o texto */
void organizeAuxForSubstitution(char *aux) {
    char index_buffer [8], from [8192], to [8192];

    sscanf(aux, "%s %s %s", index_buffer, from, to);

    if (isEqual(index_buffer, "%") == 0) {
        substituteFirstWordOcurrence(atoi(index_buffer) - 1, from, to);
        return;
    }

    for (int i = 0; i < text.totalLines; i++) {
        if (locateWord(text.lines[i].string, from) == NOT_FOUND)
            continue;

        substituteFirstWordOcurrence(i, from, to);
    }
}

/* Substitui o primeiro padrao de uma linha por outra coisa */
void substituteFirstWordOcurrence(Index line, char *from, char *to) {
    char before_word[8192] = {}, after_word[8192] = {};

    int word_pos = locateWord(text.lines[line].string, from);

    /* before_word = texto antes do padrao que quer substituir */
    strncpy(before_word, text.lines[line].string, word_pos);

    /* after_word = texto após o padrão que quer substituir */
    strcpy(after_word, &text.lines[line].string[word_pos + strlen(from)]);

    /* reescreve a linha com o padrão substituido */
    sprintf(text.lines[line].string, "%s%s%s", before_word, to, after_word);
}

/****** LOCATE COMMAND ******/
/* Função base para o comando de localizar */
void locateCmd(char *aux, Index line) {
    int totalPatterns = 0;
    for (int i = 0; i < text.totalLines; i++) {
        line = locateWord(text.lines[i].string, aux);
        if (line != -1) {
            printLine(i);
            totalPatterns++;
        }
    }
    printf("%d\n", totalPatterns);
}

/* Localiza o início de uma palavra em uma string */
int locateWord(char *str, char *word) {
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

/****** DELETE COMMAND ******/
/* Deleta uma linha */
void deleteLine(Index line) {
    if (line > text.totalLines) {
        return;
    }
    for (int i=line-1; i < text.totalLines; i++) {
        strcpy(text.lines[i].string, text.lines[i+1].string);
    }
    strcpy(text.lines[text.totalLines-1].string, "\0");
    text.totalLines--;
}

/* Verifica se duas strings são iguais */
int isEqual(char *string1, char *string2) {
    if (strcmp(string1, string2) == 0) {
        return 1;
    }
    return 0;
}
