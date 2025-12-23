#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

// Design Functions
    // Print letters from A --> H
void print_letter() {
    wprintf(L" ");
    for(int letter = 0; letter < 8; letter++) {
        wprintf(L"   ");
        wprintf(L"%lc", letter+65);
    }
    wprintf(L"\n");
}
    // Upper and lower board
void border(int up) {

    if(up) {
        wprintf(L"  ┌");
            for(int i = 0; i < 31; i++)
                wprintf(L"─");
        wprintf(L"┐\n");
    }
    else {
        wprintf(L"  └");
        for(int i = 0; i < 31; i++)
            wprintf(L"─");
        wprintf(L"┘\n");
    }

}

// the initialization of the board
void init_board(wchar_t **board) {

    for(int row = 0; row < 8; row++) {

        for(int col = 0; col < 8; col++) {

            if(row == 0) {
                if(col == 0 || col == 7)
                    board[row][col] = L'♜';
                else if(col == 1 || col == 6)
                    board[row][col] = L'♞';
                else if(col == 2 || col == 5)
                    board[row][col] = L'♝';
                else if (col == 3)
                    board[row][col] = L'♛';
                else
                    board[row][col] = L'♚'; 
            }
            else if (row == 7) {
                if(col == 0 || col == 7)
                    board[row][col] = L'♖';
                else if(col == 1 || col == 6)
                    board[row][col] = L'♘';
                else if(col == 2 || col == 5)
                    board[row][col] = L'♗';
                else if (col == 3)
                    board[row][col] = L'♕';
                else
                    board[row][col] = L'♔'; 
            }
            else if (row == 1)
                board[row][col] = L'♟';
            else if(row == 6)
                board[row][col] = L'♙'; 
            else if((row+col)%2 == 0)
                board[row][col] = L'□';
            else
                board[row][col] = L'■';

        }

    }

}

// print the provided board
// this function need to get the board as a parameter when called!
void print_board(wchar_t **board) {

    print_letter();
    border(1);

    for(int row = 0; row < 8; row++) {
            wprintf(L"%d │", row+1);
        for(int col = 0; col < 8; col++) {
            wprintf(L" %lc │", board[row][col]);
        }
        wprintf(L"\n");
    }

    border(0);

    
}

// Create the board and return it, so you can save the address 
// (it don't print the board)
wchar_t **create_board() {

    wchar_t **board = (wchar_t **) calloc(8, sizeof(wchar_t*));

    if(board == NULL) {
        printf("ERROR: There is no free space for the board!");
        exit(0);
    }

    for(int i = 0; i < 8; i++) {
        board[i] = (wchar_t *) calloc(8, sizeof(wchar_t));
        if(board[i] == NULL) {
            printf("ERROR: There is no free space for the board!");
            exit(0);
        }
    }

    setlocale(LC_ALL, "");
    init_board(board);
    
    return board;

}

// Free the board from the memory
void remove_board(wchar_t **board) {

    free(board);

}