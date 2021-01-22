#include <stdio.h>
#include <ncurses.h>

#define MAX_CHARS 600000

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
    row ++;

    char chars[MAX_CHARS];
    char ch;
    int i = 0;
    do {
        ch = getch();
        switch (ch) {
            case 10:
                wmove(stdscr, row, 0);
                row ++;
                chars[i] = '\n';
                break;
            case 'B':
                chars[i] = '\0';
                export(chars);
            default:
                chars[i] = ch;
                break;
        }
        i++;
    } while (ch != 'A');
    endScreen();
}

void export(char *chars) {
    FILE *fp = fopen("teste.txt", "w");
    fputs(chars, fp);
}
