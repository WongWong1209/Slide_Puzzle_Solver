#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct Node Node;
typedef struct Queue Queue;
void SolvePuzzle(int **board, int scale);
void moveUp(int **board, int scale, int *zero_pos, int *num_pos, int num, bool record_step);
void moveDown(int **board, int scale, int *zero_pos, int *num_pos, int num, bool record_step);
void moveLeft(int **board, int scale, int *zero_pos, int *num_pos, int num, bool record_step);
void moveRight(int **board, int scale, int *zero_pos, int *num_pos, int num, bool record_step);
void printBoard(int **board, int scale);
void getNumPosition(int num, int scale, int **board, int *num_pos);
void adjustRow(int **board, int scale, int *zero_pos, int *num_pos, int num, int i, int fix3, int fix4, int fix5, int fix6, int j);
void adjustCol(int **board, int scale, int *zero_pos, int *num_pos, int num, int j, int fix1, int fix2, int fix6, int i, int start_row, int fix3);
void swapBoard(int x1, int y1, int x2, int y2, int **board);
bool isSolved(int **board, int scale, int start_row, int start_col, int **original_board, Queue q, int num);
void init_queue(Queue *queue);
void enqueue(Queue *q, int **board, int scale);
int** dequeue(Queue *q);
bool is_in_queue(Queue *q, int **board);
void copy_board(int **board1, int **board2, int scale);
void secondStage(int **board, int scale, int num, int num_pos[2]);
void try_to_move(int **cur_board, int scale, int zero_pos[2], int num_pos[2], int num, Queue *visited_list, Queue *q, char move);

char *file_path = "./init/initial_03X03.txt";
char *log_path = "./result/log.txt";
char *step_path = "./result/steps.txt";
FILE *fptr;
FILE *resultFilePtr;
FILE *stepFilePtr;
int stopPoint;
int lastAdjust = 0; // 0: row, 1: column
int step = 0;

struct Node {
    int **board;
    char *moves;
    int step;
    Node *next;
};

struct Queue {
    int length;
    Node *front;
    Node *rear;
};

void init_queue(Queue *queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->length = 0;
}

void enqueue(Queue *q, int **board, int scale) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        printf("Memory error\n");
        exit(1);
    }
    //new_node->board = board;
    copy_board(new_node->board, board, scale);
    new_node->moves = (char *)malloc(1);
    if (!new_node->moves) {
        printf("Memory error\n");
        exit(1);
    }
    new_node->moves[0] = '\0';
    new_node->step = 0;
    new_node->next = NULL;

    q->length++;

    if (q->rear == NULL) {
        q->front = new_node;
    } else {
        q->rear->next = new_node;
    }
    q->rear = new_node;
}

int** dequeue(Queue *q) {
    if (q->front == NULL) {
        printf("Error: Queue is empty\n");
        return NULL;
    }

    int **board = q->front->board;
    struct Node *temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL) q->rear = NULL;
    q->length--;

    free(temp);
    return board;
}

bool is_in_queue(Queue *q, int **board) {
    Node *temp = q->front;
    while (temp!= NULL) {
        if (temp->board == board) return true;
        temp = temp->next;
    }
    return false;
}

void copy_board(int **board1, int **board2, int scale) {
    for (int i = 0; i < scale; i++) {
        for (int j = 0; j < scale; j++) {
            board2[i][j] = board1[i][j];
        }
    }
}

int main() {
    srand(time(NULL));
    char scale_temp[10];
    int scale;
    fptr = fopen(file_path, "r");
    resultFilePtr = fopen(log_path, "w");
    stepFilePtr = fopen(step_path, "w");

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
    fprintf(resultFilePtr, "Initial board:\n");
    for (int i = 0; i < scale; i++) {
        char line[100];
        fgets(line, sizeof(line), fptr);
        char *token = strtok(line, " \t\n");
        int j = 0;
        while (token != NULL && j < scale) {
            board[i][j] = atoi(token);
            fprintf(resultFilePtr, "%5d", board[i][j]);
            token = strtok(NULL, " \t\n");
            j++;
        }
        fprintf(resultFilePtr, "\n");
    }
    puts("");
    fclose(fptr);

    Node n = {board, "", 0, NULL};
    printf("size of node: %lu\n\n\n", sizeof(n));

    SolvePuzzle(board, scale);

    //free board
    for (int i = 0; i < scale; i++) free(board[i]);
    free(board);

    fclose(resultFilePtr);
    fclose(stepFilePtr);

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
                
                fprintf(resultFilePtr, "=======================================================================================================\n");
                fprintf(resultFilePtr, "=======================================================================================================\n");
                fprintf(resultFilePtr, "=======================================================================================================\n");
                fprintf(resultFilePtr, "=======================================================================================================\n");
                fprintf(resultFilePtr, "=======================================================================================================\n");
                fprintf(resultFilePtr, "=======================================================================================================\n");
                fprintf(resultFilePtr, "Goal: %d\n", num);

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
                    while(zero_pos[1] < scale - 2) moveRight(board, scale, zero_pos, num_pos, num, true);
                    while(zero_pos[1] > scale - 2) moveLeft(board, scale, zero_pos, num_pos, num, true);
                    while(zero_pos[0] > start_row) moveUp(board, scale, zero_pos, num_pos, num, true);
                    moveRight(board, scale, zero_pos, num_pos, num, true);
                    moveDown(board, scale, zero_pos, num_pos, num, true);
                }
                else if(lastAdjust == 3) {
                    while(zero_pos[1] > scale - 2) moveLeft(board, scale, zero_pos, num_pos, num, true);
                    while(zero_pos[1] < scale - 2) moveRight(board, scale, zero_pos, num_pos, num, true);
                    while(zero_pos[0] > i + 1) moveUp(board, scale, zero_pos, num_pos, num, true);
                    moveUp(board, scale, zero_pos, num_pos, num, true);
                    moveLeft(board, scale, zero_pos, num_pos, num, true);
                    moveDown(board, scale, zero_pos, num_pos, num, true);
                    moveDown(board, scale, zero_pos, num_pos, num, true);
                    moveRight(board, scale, zero_pos, num_pos, num, true);
                    moveRight(board, scale, zero_pos, num_pos, num, true);
                    moveUp(board, scale, zero_pos, num_pos, num, true);
                    moveLeft(board, scale, zero_pos, num_pos, num, true);
                    moveLeft(board, scale, zero_pos, num_pos, num, true);
                    moveUp(board, scale, zero_pos, num_pos, num, true);
                    moveRight(board, scale, zero_pos, num_pos, num, true);
                    moveRight(board, scale, zero_pos, num_pos, num, true);
                    moveDown(board, scale, zero_pos, num_pos, num, true);
                }
            }
            else if(i == scale - 1) {
                if(lastAdjust == 0 || lastAdjust == 1){
                    while(zero_pos[0] > scale - 2) moveUp(board, scale, zero_pos, num_pos, num, true);
                    while(zero_pos[0] < scale - 2) moveDown(board, scale, zero_pos, num_pos, num, true);
                    while(zero_pos[1] > j) moveLeft(board, scale, zero_pos, num_pos, num, true);
                    moveDown(board, scale, zero_pos, num_pos, num, true);
                    moveRight(board, scale, zero_pos, num_pos, num, true);
                }
                else  if(lastAdjust == 4) {
                    while(zero_pos[0] > scale - 2) moveUp(board, scale, zero_pos, num_pos, num, true);
                    while(zero_pos[0] < scale - 2) moveDown(board, scale, zero_pos, num_pos, num, true);
                    while(zero_pos[1] > j + 1) moveLeft(board, scale, zero_pos, num_pos, num, true);
                    moveLeft(board, scale, zero_pos, num_pos, num, true);
                    moveUp(board, scale, zero_pos, num_pos, num, true);
                    moveRight(board, scale, zero_pos, num_pos, num, true);
                    moveRight(board, scale, zero_pos, num_pos, num, true);
                    moveDown(board, scale, zero_pos, num_pos, num, true);
                    moveDown(board, scale, zero_pos, num_pos, num, true);
                    moveLeft(board, scale, zero_pos, num_pos, num, true);
                    moveUp(board, scale, zero_pos, num_pos, num, true);
                    moveUp(board, scale, zero_pos, num_pos, num, true);
                    moveLeft(board, scale, zero_pos, num_pos, num, true);
                    moveDown(board, scale, zero_pos, num_pos, num, true);
                    moveDown(board, scale, zero_pos, num_pos, num, true);
                    moveRight(board, scale, zero_pos, num_pos, num, true);
                }
            }
            if(num == stopPoint) break;
        }
        if(num == stopPoint) break;

        start_row++;
        start_col++;

        if(start_row == scale - 2 && start_col == scale - 2) break;
    }

    printf("\nEND FIRST\n");
    fprintf(resultFilePtr, "\nEND FIRST\n");
    secondStage(board, scale, num, num_pos);

    fprintf(resultFilePtr, "Final board:\n");
    printBoard(board, scale);
    printf("TOTAL STEP: %d\n", step);
    fprintf(resultFilePtr, "\n==--==--==--==--==--==--==--==--==\n");
    fprintf(resultFilePtr, ">>> TOTAL STEP: %d\n", step);
    fprintf(resultFilePtr, "==--==--==--==--==--==--==--==--==");
    fprintf(stepFilePtr, "\n==--==--==--==--==--==--==--==--==\n");
    fprintf(stepFilePtr, ">>> TOTAL STEP: %d\n", step);
    fprintf(stepFilePtr, "==--==--==--==--==--==--==--==--==");
}

void secondStage(int **board, int scale, int num, int num_pos[2]) {
    printf("SECOND STAGE\n");
    fprintf(resultFilePtr, "\nSECOND STAGE\n");
    int start_row = scale - 2;
    int start_col = scale - 3;

    Queue q;
    init_queue(&q);
    enqueue(&q, board, scale);
    printf("q's length: %d\n", q.length);
    printf("q's size: %lu\n", sizeof(q));

    Queue visited_list;
    init_queue(&visited_list);

    while(true) {
        puts("AWD");
        int **cur_board = malloc(scale * sizeof(int *));
        for (int i = 0; i < scale; i++) cur_board[i] = malloc(scale * sizeof(int));
        copy_board(dequeue(&q), cur_board, scale);
        if(cur_board == NULL) break;
        puts("GET cur_board");

        int zero_pos[2];
        getNumPosition(0, scale, cur_board, zero_pos);
        printf("zero_pos is x:%d, y:%d\nscale is %d\n", zero_pos[0], zero_pos[1], scale);

        if(zero_pos[0] != scale - 2) {
            try_to_move(cur_board, scale, zero_pos, num_pos, num, &visited_list, &q, 'U');
            printf("CHECK::::\n");
            printBoard(q.rear->board, scale);
            if(isSolved(q.rear->board, scale, start_row, start_col, board, q, num)) break;
        }
        if(zero_pos[0] != scale - 1) {
            try_to_move(cur_board, scale, zero_pos, num_pos, num, &visited_list, &q, 'D');
            printf("CHECK::::\n");
            //printBoard(q.rear->board, scale);
            int *a = &q.rear->board;
            printf("REAR: %d\n", a);
            if(isSolved(q.rear->board, scale, start_row, start_col, board, q, num)) break;
        }
        if(zero_pos[1] != scale - 3) {
            try_to_move(cur_board, scale, zero_pos, num_pos, num, &visited_list, &q, 'L');
            if(isSolved(q.rear->board, scale, start_row, start_col, board, q, num)) break;
        }
        if(zero_pos[1] != scale - 1) {
            try_to_move(cur_board, scale, zero_pos, num_pos, num, &visited_list, &q, 'R');
            if(isSolved(q.rear->board, scale, start_row, start_col, board, q, num)) break;
        }

        printf("q's length: %d\n", q.length);
        printf("q's size: %lu\n", sizeof(q));
    }
}

void try_to_move(int **cur_board, int scale, int zero_pos[2], int num_pos[2], int num, Queue *visited_list, Queue *q, char move) {
    puts("TRY");
    int **new_board = malloc(scale * sizeof(int *));
    for (int i = 0; i < scale; i++) new_board[i] = malloc(scale * sizeof(int));
    copy_board(cur_board, new_board, scale);

    switch (move) {
        case 'U':
            moveUp(new_board, scale, zero_pos, num_pos, num, false);
            break;
        case 'D':
            moveDown(new_board, scale, zero_pos, num_pos, num, false);
            break;
        case 'L':
            moveLeft(new_board, scale, zero_pos, num_pos, num, false);
            break;
        case 'R':
            moveRight(new_board, scale, zero_pos, num_pos, num, false);
            break;
    }

    if(!is_in_queue(visited_list, new_board)) {
        enqueue(visited_list, new_board, scale);
        enqueue(q, new_board, scale);
        size_t len = strlen(q->rear->moves);
        char *new_moves = (char *)realloc(q->rear->moves, len + 2);
        if (!new_moves) {
            printf("Memory error\n");
            exit(1);
        }
        q->rear->moves = new_moves;
        q->rear->moves[len] = move;
        q->rear->moves[len + 1] = '\0';
        q->rear->step++;
    }

    free(new_board);
}

bool isSolved(int **board, int scale, int start_row, int start_col, int **original_board, Queue q, int num) {
    puts("START CHECK SOLVED");
    for(int i = start_row; i < scale; i++) {
        for(int j = start_col; j < scale; j++) {
            if(board[i][j] != j + 1 + i * scale && board[i][j] != 0) return false;
        }
    }

    puts("is_solved!!!!");
    printf("%s\n", q.rear->moves);

    int zero_pos[2];
    int num_pos[2];
    getNumPosition(0, scale, original_board, zero_pos);
    getNumPosition(num, scale, original_board, num_pos);

    for(int i = 0; i < q.rear->step; i++) {
        switch(q.rear->moves[i]) {
            case 'U':
                moveDown(original_board, scale, zero_pos, num_pos, num, true);
                break;
            case 'D':
                moveUp(original_board, scale, zero_pos, num_pos, num, true);
                break;
            case 'L':
                moveRight(original_board, scale, zero_pos, num_pos, num, true);
                break;
            case 'R':
                moveLeft(original_board, scale, zero_pos, num_pos, num, true);
                break;
        }
    }
    return true;
}

void adjustRow(int **board, int scale, int *zero_pos, int *num_pos, int num, int i, int fix3, int fix4, int fix5, int fix6, int j) {
    printf("A\n");
    while(num_pos[0] != i + fix3 + fix5) {
        if(fix6 && num_pos[0] == scale - 2 && num_pos[1] == j) {
            lastAdjust = 4;
            break;
        }
        if(fix3 && num_pos[0] == i && num_pos[1] == scale - 2) {
            lastAdjust = 3;
            break;
        }
        while(zero_pos[0] > num_pos[0] - fix4) moveUp(board, scale, zero_pos, num_pos, num, true);
        while(zero_pos[0] < num_pos[0] - fix4) moveDown(board, scale, zero_pos, num_pos, num, true);
        while(zero_pos[1] < num_pos[1]) moveRight(board, scale, zero_pos, num_pos, num, true);
        while(zero_pos[1] > num_pos[1]) moveLeft(board, scale, zero_pos, num_pos, num, true);
        if(fix4 == 1) moveDown(board, scale, zero_pos, num_pos, num, true);
        else moveUp(board, scale, zero_pos, num_pos, num, true);
        if(zero_pos[1] != scale - 1) {
            moveRight(board, scale, zero_pos, num_pos, num, true);
            moveDown(board, scale, zero_pos, num_pos, num, true);
            moveDown(board, scale, zero_pos, num_pos, num, true);
        }
        else moveLeft(board, scale, zero_pos, num_pos, num, true);
        lastAdjust = 0;
        fprintf(resultFilePtr, "======================================================\n");
        fprintf(resultFilePtr, "================                      ================\n");
        fprintf(resultFilePtr, "================      Adjust Row      ================\n");
        fprintf(resultFilePtr, "================                      ================\n");
        fprintf(resultFilePtr, "======================================================\n");
    }
}

void adjustCol(int **board, int scale, int *zero_pos, int *num_pos, int num, int j, int fix1, int fix2, int fix6, int i, int start_row, int fix3) {
    printf("B\n");
    while(num_pos[1] != j + fix2 + fix6) {
        if(fix6 && num_pos[0] == scale - 2 && num_pos[1] == j) {
            lastAdjust = 4;
            break;
        }
        if(fix3 && num_pos[0] == i && num_pos[1] == scale - 2) {
            lastAdjust = 3;
            break;
        }
        while(zero_pos[1] > num_pos[1] + fix1) moveLeft(board, scale, zero_pos, num_pos, num, true);
        while(zero_pos[1] < num_pos[1] + fix1) moveRight(board, scale, zero_pos, num_pos, num, true);
        while(zero_pos[0] < num_pos[0]) moveDown(board, scale, zero_pos, num_pos, num, true);
        while(zero_pos[0] > num_pos[0]) moveUp(board, scale, zero_pos, num_pos, num, true);
        if(fix1 == 1) moveLeft(board, scale, zero_pos, num_pos, num, true);
        else moveRight(board, scale, zero_pos, num_pos, num, true);
        if(zero_pos[0] != scale - 1) {
            moveDown(board, scale, zero_pos, num_pos, num, true);
            moveRight(board, scale, zero_pos, num_pos, num, true);
            moveRight(board, scale, zero_pos, num_pos, num, true);
        }
        else {
            moveUp(board, scale, zero_pos, num_pos, num, true);
            moveRight(board, scale, zero_pos, num_pos, num, true);
            moveRight(board, scale, zero_pos, num_pos, num, true);
        }
        lastAdjust = 1;
        fprintf(resultFilePtr, "======================================================\n");
        fprintf(resultFilePtr, "================                      ================\n");
        fprintf(resultFilePtr, "================     Adjust Column    ================\n");
        fprintf(resultFilePtr, "================                      ================\n");
        fprintf(resultFilePtr, "======================================================\n");
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

void moveUp(int **board, int scale, int *zero_pos, int *num_pos, int num, bool record_step) {
    if (zero_pos[0] > 0) {
        fprintf(resultFilePtr, "Move Up:\n");
        int x = zero_pos[0];
        int y = zero_pos[1];
        swapBoard(x-1, y, x, y, board);
        zero_pos[0]--;
        if(record_step) {
            step++;
            printBoard(board, scale);
        }
        getNumPosition(num, scale, board, num_pos);
    }
}

void moveDown(int **board, int scale, int *zero_pos, int *num_pos, int num, bool record_step) {
    if (zero_pos[0] < scale - 1) {
        fprintf(resultFilePtr, "Move Down:\n");
        int x = zero_pos[0];
        int y = zero_pos[1];
        swapBoard(x+1, y, x, y, board);
        zero_pos[0]++;
        if(record_step) {
            step++;
            printBoard(board, scale);
        }
        getNumPosition(num, scale, board, num_pos);
    }
}

void moveRight(int **board, int scale, int *zero_pos, int *num_pos, int num, bool record_step) {
    if (zero_pos[1] < scale - 1) {
        fprintf(resultFilePtr, "Move Right:\n");
        int x = zero_pos[0];
        int y = zero_pos[1];
        swapBoard(x, y+1, x, y, board);
        zero_pos[1]++;
        if(record_step) {
            step++;
            printBoard(board, scale);
        }
        getNumPosition(num, scale, board, num_pos);
    }
}

void moveLeft(int **board, int scale, int *zero_pos, int *num_pos, int num, bool record_step) {
    if (zero_pos[1] > 0) {
        fprintf(resultFilePtr, "Move Left:\n");
        int x = zero_pos[0];
        int y = zero_pos[1];
        swapBoard(x, y-1, x, y, board);
        zero_pos[1]--; 
        if(record_step) {
            step++;
            printBoard(board, scale);
        }
        getNumPosition(num, scale, board, num_pos);
    }
}

void swapBoard(int x1, int y1, int x2, int y2, int **board) {
    int temp = board[x1][y1];
    board[x1][y1] = board[x2][y2];
    board[x2][y2] = temp;
}

void printBoard(int **board, int scale) {
    for(int i=0;i<scale;i++) {
        for(int j=0;j<scale;j++) {
            printf("%5d ", board[i][j]);
            fprintf(resultFilePtr, "%5d ", board[i][j]);
        }
        printf("\n");
        fprintf(resultFilePtr, "\n");
    }
    printf("\n");
}