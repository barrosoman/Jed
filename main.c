#include <stdio.h>
#include <ncurses.h>

#define MAX_CHARS 600000

typedef struct letra{
    char ch;
    struct letra *prox;
    struct letra *prev;
} letra;

void initScreen();
void endScreen();
void writeLoop();
void export(char *chars);

int main() {

    initScreen();

    refresh();

    writeLoop();

    return 0;
}

void initScreen() {
    initscr();
    keypad(stdscr, TRUE);
    raw();
    echo();
}

void endScreen() {
    endwin();
}

void writeLoop() {
    int row, col;
    getyx(stdscr, row, col);
    col ++;
    row ++;

    do {
        letter.ch = getch();
        switch (letter.ch) {
            case 10:
                wmove(stdscr, row, 0);
                row ++;
                letter.ch = '\n';
                break;
            case 'B':
                letter.ch = '\0';
                export(chars);
            default:
                letter.ch = ch;
                break;
        }
    } while (ch != 'A');
    endScreen();
}

void export(char *chars) {
    FILE *fp = fopen("teste.txt", "w");
    fputs(chars, fp);
}
