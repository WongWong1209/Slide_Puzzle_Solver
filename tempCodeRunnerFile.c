char initial_moves[10000] = "";
    // char queue[10000][10000];
    // int front = 0, rear = 0;
    // start_row = scale - 2;
    // start_col = scale - 3;

    // strcpy(queue[rear++], initial_moves);

    // while (front < rear) {
    //     char current_moves[10000];
    //     strcpy(current_moves, queue[front++]);

    //     if (isSolved(board, scale, start_row, start_col)) {
    //         printf("Solved puzzle with moves: %s\n", current_moves);
    //         break;
    //     }

    //     if (zero_pos[0] > start_row) {
    //         moveUp(board, scale, zero_pos, num_pos, num);
    //         strcpy(queue[rear++], current_moves);
    //         strcat(queue[rear - 1], "U");
    //     }
    //     if (zero_pos[0] < start_row + 1) {
    //         moveDown(board, scale, zero_pos, num_pos, num);
    //         strcpy(queue[rear++], current_moves);
    //         strcat(queue[rear - 1], "D");
    //     }
    //     if (zero_pos[1] > start_col) {
    //         moveLeft(board, scale, zero_pos, num_pos, num);
    //         strcpy(queue[rear++], current_moves);
    //         strcat(queue[rear - 1], "L");
    //     }
    //     if (zero_pos[1] < start_col + 2) {
    //         moveRight(board, scale, zero_pos, num_pos, num);
    //         strcpy(queue[rear++], current_moves);
    //         strcat(queue[rear - 1], "R");
    //     }

    //     for (int i = 0; i < strlen(current_moves); ++i) {
    //         char move = current_moves[i];
    //         switch (move) {
    //             case 'U': moveDown(board, scale, zero_pos, num_pos, num); break;
    //             case 'D': moveUp(board, scale, zero_pos, num_pos, num); break;
    //             case 'L': moveRight(board, scale, zero_pos, num_pos, num); break;
    //             case 'R': moveLeft(board, scale, zero_pos, num_pos, num); break;
    //         }
    //     }
    // }