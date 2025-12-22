#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#include "../include/board.h"
#include "../include/system.h"

int main() {

    wchar_t **board = create_board();

    main_menu(board);
    start(board);
    
}