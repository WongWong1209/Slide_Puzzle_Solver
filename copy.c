#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>#include <unistd.h>

void sleep_ms(int milliseconds) {
    usleep(milliseconds * 1000);  // Convert to microseconds
}

void SolvePuzzle(int **board, int scale);
void moveUp(int **board, int scale, int *zero_pos, int *num_pos, int num);
void moveDown(int **board, int scale, int *zero_pos, int *num_pos, int num);
void moveLeft(int **board, int scale, int *zero_pos, int *num_pos, int num);
void moveRight(int **board, int scale, int *zero_pos, int *num_pos, int num);
void printBoard(int **board, int scale);
void getNumPosition(int num, int scale, int **board, int *num_pos);
void adjustRow(int **board, int scale, int *zero_pos, int *num_pos, int num, int i, int fix3, int fix4, int fix5, int fix6, int j);
void adjustCol(int **board, int scale, int *zero_pos, int *num_pos, int num, int j, int fix1, int fix2, int fix6, int i, int start_row, int fix3);
void swapBoard(int x1, int y1, int x2, int y2, int **board);
bool isSolved(int **board, int scale, int start_row, int start_col);

char *file_path = "./init/initial_13X13.txt";
FILE *fptr;
int stopPoint;
int lastAdjust = 0; // 0: row, 1: column
int step = 0;
int step2 = 0;


int main() {
    srand(time(NULL));
    char scale_temp[10];
    int scale;
    fptr = fopen(file_path, "r");

    if (!fptr) {
        printf("Error: Could not open file %s\n", file_path);
        return 1;
    }

    fgets(scale_temp, 10, fptr);
    scale = atoi(scale_temp);
    stopPoint = scale * scale - 2 * scale;
    //stopPoint =46;

    //set board size
    int **board = malloc(scale * sizeof(int *));
    for (int i = 0; i < scale; i++) board[i] = malloc(scale * sizeof(int));

    //set initial board
    for (int i = 0; i < scale; i++) {
        char line[100];
        fgets(line, sizeof(line), fptr);
        char *token = strtok(line, " \t\n");
        int j = 0;
        while (token != NULL && j < scale) {
            board[i][j] = atoi(token);
            token = strtok(NULL, " \t\n");
            j++;
        }
        printBoard(board, scale);
    }
    fclose(fptr);

    SolvePuzzle(board, scale);

    //free board
    for (int i = 0; i < scale; i++) free(board[i]);
    free(board);

    return 0;
}

void SolvePuzzle(int **board, int scale) {
    int start_row = 0, start_col = 0;
    int num;
    int num_pos[2]; // 0: row, 1: column
    int zero_pos[2]; // 0: row, 1: column
    int i, j;

    while(1) {
        for(int i = start_row; i < scale; i++) {
            for(j = start_col; j < scale; j++) {
                if(start_row == scale - 3 && i > start_row) break;

                if(num == stopPoint) break;
                num = j + 1 + i * scale;

                getNumPosition(num, scale, board, num_pos);
                getNumPosition(0, scale, board, zero_pos);

                int fix2 = j == scale-2 ? 1 : 0;
                int fix1 = num_pos[1] < j + fix2 ? 1 : -1;
                int fix3 = j == scale-1 ? 1 : 0;
                int fix4 = num_pos[0] > i ? 1 : -1;
                int fix5 = i == scale-2 ? 1 : 0;
                int fix6 = i == scale-1 ? 1 : 0;

                if(i == start_row) {
                    adjustCol(board, scale, zero_pos, num_pos, num, j, fix1, fix2, fix6, i, start_row, fix3);
                    adjustRow(board, scale, zero_pos, num_pos, num, i, fix3, fix4, fix5, fix6, j);
                }
                else {
                    adjustRow(board, scale, zero_pos, num_pos, num, i, fix3, fix4, fix5, fix6, j);
                    adjustCol(board, scale, zero_pos, num_pos, num, j, fix1, fix2, fix6, i, start_row, fix3);
                }

                if(i > start_row) break;
            }

            if(i == start_row) {
                if(lastAdjust == 0 || lastAdjust == 1) {
                    while(zero_pos[1] < scale - 2) moveRight(board, scale, zero_pos, num_pos, num);
                    while(zero_pos[1] > scale - 2) moveLeft(board, scale, zero_pos, num_pos, num);
                    while(zero_pos[0] > start_row) moveUp(board, scale, zero_pos, num_pos, num);
                    moveRight(board, scale, zero_pos, num_pos, num);
                    moveDown(board, scale, zero_pos, num_pos, num);
                }
                else if(lastAdjust == 3) {
                    while(zero_pos[1] > scale - 2) moveLeft(board, scale, zero_pos, num_pos, num);
                    while(zero_pos[1] < scale - 2) moveRight(board, scale, zero_pos, num_pos, num);
                    while(zero_pos[0] > i + 1) moveUp(board, scale, zero_pos, num_pos, num);
                    moveUp(board, scale, zero_pos, num_pos, num);
                    moveLeft(board, scale, zero_pos, num_pos, num);
                    moveDown(board, scale, zero_pos, num_pos, num);
                    moveDown(board, scale, zero_pos, num_pos, num);
                    moveRight(board, scale, zero_pos, num_pos, num);
                    moveRight(board, scale, zero_pos, num_pos, num);
                    moveUp(board, scale, zero_pos, num_pos, num);
                    moveLeft(board, scale, zero_pos, num_pos, num);
                    moveLeft(board, scale, zero_pos, num_pos, num);
                    moveUp(board, scale, zero_pos, num_pos, num);
                    moveRight(board, scale, zero_pos, num_pos, num);
                    moveRight(board, scale, zero_pos, num_pos, num);
                    moveDown(board, scale, zero_pos, num_pos, num);
                }
            }
            else if(i == scale - 1) {
                if(lastAdjust == 0 || lastAdjust == 1){
                    while(zero_pos[0] > scale - 2) moveUp(board, scale, zero_pos, num_pos, num);
                    while(zero_pos[0] < scale - 2) moveDown(board, scale, zero_pos, num_pos, num);
                    while(zero_pos[1] > j) moveLeft(board, scale, zero_pos, num_pos, num);
                    moveDown(board, scale, zero_pos, num_pos, num);
                    moveRight(board, scale, zero_pos, num_pos, num);
                }
                else  if(lastAdjust == 4) {
                    while(zero_pos[0] > scale - 2) moveUp(board, scale, zero_pos, num_pos, num);
                    while(zero_pos[0] < scale - 2) moveDown(board, scale, zero_pos, num_pos, num);
                    while(zero_pos[1] > j + 1) moveLeft(board, scale, zero_pos, num_pos, num);
                    moveLeft(board, scale, zero_pos, num_pos, num);
                    moveUp(board, scale, zero_pos, num_pos, num);
                    moveRight(board, scale, zero_pos, num_pos, num);
                    moveRight(board, scale, zero_pos, num_pos, num);
                    moveDown(board, scale, zero_pos, num_pos, num);
                    moveDown(board, scale, zero_pos, num_pos, num);
                    moveLeft(board, scale, zero_pos, num_pos, num);
                    moveUp(board, scale, zero_pos, num_pos, num);
                    moveUp(board, scale, zero_pos, num_pos, num);
                    moveLeft(board, scale, zero_pos, num_pos, num);
                    moveDown(board, scale, zero_pos, num_pos, num);
                    moveDown(board, scale, zero_pos, num_pos, num);
                    moveRight(board, scale, zero_pos, num_pos, num);
                }
            }
            if(num == stopPoint) break;
        }
        if(num == stopPoint) break;

        start_row++;
        start_col++;

        if(start_row == scale - 2 && start_col == scale - 2) break;
    }

    start_row = scale - 2;
    start_col = scale - 3;
    int last_move = 0;
    step2 = 0;

    while(!isSolved(board, scale, start_row, start_col)) {
        int cur_move; // 0: up, 1: right, 2: down, 3: left
        do { cur_move = rand() % 4; } while(abs(cur_move-last_move) == 2);

        last_move = cur_move;

        switch(cur_move) {
            case 0:
                if(zero_pos[0] != start_row) moveUp(board, scale, zero_pos, num_pos, num);
                break;
            case 1:
                if(zero_pos[1] != scale - 1) moveRight(board, scale, zero_pos, num_pos, num);
                break;
            case 2:
                if(zero_pos[0] != scale - 1) moveDown(board, scale, zero_pos, num_pos, num);
                break;
            case 3:
                if(zero_pos[1] != start_col) moveLeft(board, scale, zero_pos, num_pos, num);
                break;
        }
    }

    printBoard(board, scale);
}

bool isSolved(int **board, int scale, int start_row, int start_col) {
    for(int i = start_row; i < scale; i++) {
        for(int j = start_col; j < scale; j++) {
            if(board[i][j] != j + 1 + i * scale && board[i][j] != 0) return false;
        }
    }
    return true;
}

void adjustRow(int **board, int scale, int *zero_pos, int *num_pos, int num, int i, int fix3, int fix4, int fix5, int fix6, int j) {
    while(num_pos[0] != i + fix3 + fix5) {
        if(fix6 && num_pos[0] == scale - 2 && num_pos[1] == j) {
            lastAdjust = 4;
            break;
        }
        if(fix3 && num_pos[0] == i && num_pos[1] == scale - 2) {
            lastAdjust = 3;
            break;
        }
        while(zero_pos[0] > num_pos[0] - fix4) moveUp(board, scale, zero_pos, num_pos, num);
        while(zero_pos[0] < num_pos[0] - fix4) moveDown(board, scale, zero_pos, num_pos, num);
        while(zero_pos[1] < num_pos[1]) moveRight(board, scale, zero_pos, num_pos, num);
        while(zero_pos[1] > num_pos[1]) moveLeft(board, scale, zero_pos, num_pos, num);
        if(fix4 == 1) moveDown(board, scale, zero_pos, num_pos, num);
        else moveUp(board, scale, zero_pos, num_pos, num);
        if(zero_pos[1] != scale - 1) {
            moveRight(board, scale, zero_pos, num_pos, num);
            moveDown(board, scale, zero_pos, num_pos, num);
            moveDown(board, scale, zero_pos, num_pos, num);
        }
        else moveLeft(board, scale, zero_pos, num_pos, num);
        lastAdjust = 0;
    }
}

void adjustCol(int **board, int scale, int *zero_pos, int *num_pos, int num, int j, int fix1, int fix2, int fix6, int i, int start_row, int fix3) {
    while(num_pos[1] != j + fix2 + fix6) {
        if(fix6 && num_pos[0] == scale - 2 && num_pos[1] == j) {
            lastAdjust = 4;
            break;
        }
        if(fix3 && num_pos[0] == i && num_pos[1] == scale - 2) {
            lastAdjust = 3;
            break;
        }
        while(zero_pos[1] > num_pos[1] + fix1) moveLeft(board, scale, zero_pos, num_pos, num);
        while(zero_pos[1] < num_pos[1] + fix1) moveRight(board, scale, zero_pos, num_pos, num);
        while(zero_pos[0] < num_pos[0]) moveDown(board, scale, zero_pos, num_pos, num);
        while(zero_pos[0] > num_pos[0]) moveUp(board, scale, zero_pos, num_pos, num);
        if(fix1 == 1) moveLeft(board, scale, zero_pos, num_pos, num);
        else moveRight(board, scale, zero_pos, num_pos, num);
        if(zero_pos[0] != scale - 1) {
            moveDown(board, scale, zero_pos, num_pos, num);
            moveRight(board, scale, zero_pos, num_pos, num);
            moveRight(board, scale, zero_pos, num_pos, num);
        }
        else {
            moveUp(board, scale, zero_pos, num_pos, num);
            moveRight(board, scale, zero_pos, num_pos, num);
            moveRight(board, scale, zero_pos, num_pos, num);
        }
        lastAdjust = 1;
    }
}

void getNumPosition(int num, int scale, int **board, int *num_pos) {
    for(int i = 0; i < scale; i++) {
        for(int j = 0; j < scale; j++) {
            if(board[i][j] == num) {
                num_pos[0] = i;
                num_pos[1] = j;
                return;
            }
        }
    }
}

void moveUp(int **board, int scale, int *zero_pos, int *num_pos, int num) {
    if (zero_pos[0] > 0) {
        int x = zero_pos[0];
        int y = zero_pos[1];
        swapBoard(x-1, y, x, y, board);
        zero_pos[0]--;
        step++;
        step2++;
        printBoard(board, scale);
        getNumPosition(num, scale, board, num_pos);
    }
}

void moveDown(int **board, int scale, int *zero_pos, int *num_pos, int num) {
    if (zero_pos[0] < scale - 1) {
        int x = zero_pos[0];
        int y = zero_pos[1];
        swapBoard(x+1, y, x, y, board);
        zero_pos[0]++;
        step++;
        step2++;
        printBoard(board, scale);
        getNumPosition(num, scale, board, num_pos);
    }
}

void moveRight(int **board, int scale, int *zero_pos, int *num_pos, int num) {
    if (zero_pos[1] < scale - 1) {
        int x = zero_pos[0];
        int y = zero_pos[1];
        swapBoard(x, y+1, x, y, board);
        zero_pos[1]++;
        step++;
        step2++;
        printBoard(board, scale);
        getNumPosition(num, scale, board, num_pos);
    }
}

void moveLeft(int **board, int scale, int *zero_pos, int *num_pos, int num) {
    if (zero_pos[1] > 0) {
        int x = zero_pos[0];
        int y = zero_pos[1];
        swapBoard(x, y-1, x, y, board);
        zero_pos[1]--; 
        step++;
        step2++;
        printBoard(board, scale);
        getNumPosition(num, scale, board, num_pos);
    }
}

void swapBoard(int x1, int y1, int x2, int y2, int **board) {
    int temp = board[x1][y1];
    board[x1][y1] = board[x2][y2];
    board[x2][y2] = temp;
}

void printBoard(int **board, int scale) {
    sleep_ms(50);
    printf("\033[H\033[J");
    for(int i=0;i<scale;i++) {
        for(int j=0;j<scale;j++) {
            printf("%5d ", board[i][j]);
        }
        printf("\n");
    }
}