#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#include "../include/board.h"
#include "../include/system.h"

int main() {

    wchar_t **board = create_board();
    wchar_t ***memory_board = create_board_memory();

    main_menu(board, memory_board);
    start(board, memory_board);
    
}