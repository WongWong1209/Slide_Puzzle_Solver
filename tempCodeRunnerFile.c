int **new_board;
    // copy_board(cur_board, new_board, scale);
    // switch (move) {
    //     case 'U':
    //         moveUp(new_board, scale, zero_pos, num_pos, num, false);
    //         break;
    //     case 'D':
    //         moveDown(new_board, scale, zero_pos, num_pos, num, false);
    //         break;
    //     case 'L':
    //         moveLeft(new_board, scale, zero_pos, num_pos, num, false);
    //         break;
    //     case 'R':
    //         moveRight(new_board, scale, zero_pos, num_pos, num, false);
    //         break;
    // }

    // if(!is_in_queue(visited_list, new_board)) {
    //     enqueue(visited_list, new_board);
    //     enqueue(q, new_board);
    //     size_t len = strlen(q->rear->moves);
    //     char *new_moves = (char *)realloc(q->rear->moves, len + 2);
    //     if (!new_moves) {
    //         printf("Memory error\n");
    //         exit(1);
    //     }
    //     q->rear->moves = new_moves;
    //     q->rear->moves[len] = move;
    //     q->rear->moves[len + 1] = '\0';
    //     q->rear->step++;
    // }