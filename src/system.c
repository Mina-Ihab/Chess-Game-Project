#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>

#include "../include/board.h"
#include "../include/move.h"

int saveSlot = -1;

// Make every string in upper case
void to_upper(char* string) {

    for(int i = 0; i < strlen(string); i++) {
        string[i] = toupper(string[i]);
    }

}

// Clear the terminal
void clear_terminal() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

}

// Check for any type of errors
int error_check(int msg_id) {

    // Invalid Input Error 1
    if(msg_id == 1) {
        wprintf(L"ERROR: Invalid input. Please try again.\n"); 
    }
    // Empty Place Error 2
    else if(msg_id == 2) {
        wprintf(L"ERROR: You selected an empty place. Choose a valid one.\n");
    }
    // Save Error 3
    else if(msg_id == 3) {
        wprintf(L"SYSTEM: You don't have any saved games yet.\n");
    }
    else if(msg_id == 4) {
        wprintf(L"ERROR: This is not your piece.\n");
    }
    else if(msg_id == 5) {
        wprintf(L"ERROR: Invalid movement.\n");
    }
    else if(msg_id == 6) {
        wprintf(L"SYSTEM: Your king is currently in check!\n");
    }
    else if(msg_id == 7) {
        wprintf(L"ERROR: There is no more UNDO!\n");
    }
    else if(msg_id == 8) {
        wprintf(L"ERROR: There is no more REDO!\n");
    }

    return 0;
}

// Cleaning the buffer.
// it only work when there is no '\n'.
void cleaning_buffer(char* string) {

    //If there is no end of line (that's mean there is a buffer)
    while(strchr(string, '\n') == NULL) {
        //Read the character by character
        //and remove it from the buffer
        int ch = getchar();

        //if it's a new line then this is the end of the input
        if(ch == '\n')
            break;
    }

}

// Write the board in a file 'save.txt'
void save_board(wchar_t **board) {

    // open the file
    FILE *f = fopen("./bin/save.txt", "w");
    
    for(int i = 0; i < 8; i++) {

        // write row by row each element of size wchar_t and they are 8 element
        fwrite(board[i], sizeof(wchar_t), 8, f);

    }

    // close the file
    fclose(f);

}

// load the board from the file 'save.txt'
int load_board(wchar_t **board) {

    // open the file
    FILE *f = fopen("./bin/save.txt", "r");

    // if there is no file
    if(f == NULL) {
        // it will return 3 to the error variable which will make the loop continue and send error.
        return 3;
    }
    
    for(int i = 0; i < 8; i++) {

        // read row by row each element of size wchar_t and they are 8 element
        fread(board[i], sizeof(wchar_t), 8, f);

    }

    // close the file
    fclose(f);

    //it will return 0 to error variable which will break the loop and start
    return 0;

}

// Create a memory to save boards
wchar_t*** create_board_memory() {

    wchar_t*** board_memory = (wchar_t ***) calloc(500, sizeof(wchar_t**));

    if(board_memory == NULL) {
        printf("ERROR: There is no free to space create board memory!");
        exit(0);
    }
    for(int i = 0; i < 500; i++) {
        board_memory[i] = (wchar_t **) calloc(8, sizeof(wchar_t*));
        if(board_memory[i] == NULL) {
            printf("ERROR: There is no free to space create board memory!");
            exit(0);
        }
        for(int j = 0; j < 8; j++) {
            board_memory[i][j] = (wchar_t *) calloc(8, sizeof(wchar_t));
            if(board_memory[i][j] == NULL) {
                printf("ERROR: There is no free space to create board memory!");
                exit(0);
            }
        }
    }

    return board_memory;
}

// Save the board for UNDO/REDO
int save_move(wchar_t*** memory_board, wchar_t** board) {

    saveSlot++;

    for(int r = 0; r < 8; r++)
        for(int c = 0; c < 8; c++)
            memory_board[saveSlot][c][r] = board[r][c];

}

// UNDO Move
void undo_move(wchar_t*** memory_board, wchar_t** board, int* error, int* max, int update, int* setting_arry) {

    if(saveSlot < 1){*error = 7;return;}
    saveSlot--;

    for(int r = 0; r < 8; r++)
        for(int c = 0; c < 8; c++)
            board[r][c] = memory_board[saveSlot][c][r];

    if(update == 1) saveSlot = *max;

    if(saveSlot == setting_arry[1]-1) {castle1 = 1; castle_change[1]--;}
    if(saveSlot == setting_arry[2]-1) {castle2 = 1; castle_change[2]--;}
    if(saveSlot == setting_arry[3]-1) {castle3 = 1; castle_change[3]--;}
    if(saveSlot == setting_arry[4]-1) {castle4 = 1; castle_change[4]--;}

    wprintf(L"%d: %d %d %d %d\n", saveSlot, setting_arry[1], setting_arry[2], setting_arry[3], setting_arry[4]);

}

// REDO Move
void redo_move(wchar_t*** memory_board, wchar_t** board, int max, int* error) {

    saveSlot++;

    if(saveSlot > max) {
        saveSlot--;
        *error = 8;
        return;
    }

    for(int r = 0; r < 8; r++)
        for(int c = 0; c < 8; c++)
            board[r][c] = memory_board[saveSlot][c][r];

}

void print_dead(wchar_t *b_pieces,wchar_t *w_pieces, int *w_dead_arr, int* b_dead_arr) {

    wprintf(L"White eaten: ");
    for(int i = 1; i < 6; i++) {
        wprintf(L"%lc : %d ", w_pieces[i], w_dead_arr[i]);
    }
    wprintf(L"\n");

    wprintf(L"Black eaten: ");
    for(int i = 1; i < 6; i++) {
        wprintf(L"%lc : %d ", b_pieces[i], b_dead_arr[i]);
    }
    wprintf(L"\n");

}

// Start the game
void start(wchar_t **board) {

    // 1 = White Team
    // 0 = Black Team
    int switching_team = 1;
    
    // memory for board to redo/undo
    wchar_t*** memory_board = create_board_memory();
    save_move(memory_board, board);
    int max_slot = saveSlot;

    int setting_arry[5] = {0,0,0,0,0};

    int error = 0;
    int save = 0;
    char input[6];//4 inputs + \n +\0
    wchar_t white_team[6]={L'♚', L'♛', L'♜', L'♝', L'♞', L'♟'};
    wchar_t black_team[6]={L'♔', L'♕', L'♖', L'♗', L'♘', L'♙'};

    //we made this colors to be understandable as the white seams white in vscode darkmode
    int Wdead[6]={0,0,0,0,0,0}, Bdead[6]={0,0,0,0,0,0};

    //we can remove the 1 and make it, if the game is not end 
    while(1) {

        wprintf(L"%d %d %d %d %d\n", saveSlot, castle1, castle2, castle3, castle4);
        
        if(switching_team) {

            //Clear Terminal before each print
            // clear_terminal();

            //print the board
            print_board(board);
            print_dead(black_team, white_team, Wdead, Bdead);

            //if the loop is repeated due to an error
            error = error_check(error);
            
            //if the loop is repeated due to save
            if(save) {
                wprintf(L"SYSTEM: Game saved successfully!\n");
                save_board(board);
                save = 0;
            }

            // take inpute
            wprintf(L"White Turn:\n--> ");
            fgets(input, sizeof(input), stdin);

            //Cleaning Buffer
            cleaning_buffer(input);

            //Make the letter UpperCase
            input[0] = toupper(input[0]);
            input[2] = toupper(input[2]);
            
            //Declare Co-ordination
                //For scaning:
                    char s_col, d_col;
                    int s_row, d_row;
                //Real Co-ordination:
                    int sel_col, sel_row, dest_row, dest_col;
            
            
            // check validity of input
            if(sscanf(input, "%c %d %c %d", &s_col, &s_row, &d_col, &d_row) == 4) {
                
                sel_col = s_col - 'A';
                dest_col = d_col - 'A';
                sel_row = s_row - 1;
                dest_row = d_row - 1;

            }
            else {
                // if it's not a game input
                input[1] = toupper(input[1]);
                input[3] = toupper(input[3]);

                // Check if it is a saving input
                if(strcmp(input, "SAVE\n") == 0) {
                    save = 1; 
                    continue;
                }
                // UNDO
                else if(strcmp(input, "UNDO\n") == 0) {

                    undo_move(memory_board, board, &error, &max_slot, 0, setting_arry);
                    if(error == 0) {switching_team = 0;}
                    continue;

                }
                // REDO
                else if(strcmp(input, "REDO\n") == 0) {

                    redo_move(memory_board, board, max_slot, &error);
                    if(error == 0) {switching_team = 0;}
                    continue;

                }
                // Else this is an error
                else {

                    error = 1;
                    continue;

                }
            }
            
            //Check if its in the board
            if((sel_col < 0 || sel_col > 7) || (dest_col < 0 || dest_col > 7) || (sel_row < 0 || sel_row > 7) || (dest_row < 0 || dest_row > 7)) {
                error = 1;
                continue;
            }

            //Check for empty place
            if(board[sel_row][sel_col] == L'□' || board[sel_row][sel_col] == L'■') {
                error = 2;
                continue;
            }

            //Move the piece
            movement(sel_row, sel_col, dest_row, dest_col, board, switching_team,
                     &error, white_team, black_team, Wdead, Bdead, memory_board,
                     &max_slot, setting_arry);

            if(error != 0) continue;

            //switch the team
            switching_team = 0;

        }
        else {
            
            //Clear Terminal before each print
            // clear_terminal();

            //print the board
            print_board(board);
            print_dead(black_team, white_team, Wdead, Bdead);

            //if the loop is repeated due an error
            error = error_check(error);

            //if the loop is repeated due to save
            if(save) {
                wprintf(L"SYSTEM: Game saved successfully!\n");
                save_board(board);
                save = 0;
            }

            // take inpute
            wprintf(L"Black Turn:\n--> ");
            fgets(input, sizeof(input), stdin);

            //Cleaning Buffer
            cleaning_buffer(input);

            //Make the letter UpperCase
            input[0] = toupper(input[0]);
            input[2] = toupper(input[2]);;
            
            //Declare Co-ordination
                //For scaning:
                    char s_col, d_col;
                    int s_row, d_row;
                //Real Co-ordination:
                    int sel_col, sel_row, dest_row, dest_col;
            
            
            // check validity of input
            if(sscanf(input, "%c %d %c %d", &s_col, &s_row, &d_col, &d_row) == 4) {
                
                sel_col = s_col - 'A';
                dest_col = d_col - 'A';
                sel_row = s_row - 1;
                dest_row = d_row - 1;

            }
            // If it is not a game input
            else {

                input[1] = toupper(input[1]);
                input[3] = toupper(input[3]);

                // Check if it is a saving input
                if(strcmp(input, "SAVE\n") == 0) {
                    save = 1; 
                    continue;
                }
                // UNDO
                else if(strcmp(input, "UNDO\n") == 0) {

                    undo_move(memory_board, board, &error, &max_slot, 0, setting_arry);
                    if(error == 0) {switching_team = 1;}
                    continue;

                }
                // REDO
                else if(strcmp(input, "REDO\n") == 0) {

                    redo_move(memory_board, board, max_slot, &error);
                    if(error == 0) {switching_team = 1;}
                    continue;

                }
                // Else this is an error
                else {
                    error = 1;
                    continue;
                }
            }

            
            //Check if its in the board
            if((sel_col < 0 || sel_col > 7) || (dest_col < 0 || dest_col > 7) || (sel_row < 0 || sel_row > 7) || (dest_row < 0 || dest_row > 7)) {
                error = 1;
                continue;
            }

            //Check for empty place
            if(board[sel_row][sel_col] == L'□' || board[sel_row][sel_col] == L'■') {
                error = 2;
                continue;
            }

            //Move the piece
            movement(sel_row, sel_col, dest_row, dest_col, board, switching_team,
                     &error, white_team, black_team, Wdead, Bdead, memory_board,
                     &max_slot, setting_arry);

            if(error != 0) continue;

            //switch the team
            switching_team = 1;

        }

        save_move(memory_board, board);
        max_slot = saveSlot;

    }
    
    free(memory_board);
    free(board);
}

// Main Menu
void main_menu(wchar_t **board) {

    char os_input[10];
    int saving_error = 0;

    while(1) {

        // clear_terminal();

        wprintf(L"------==MAIN MENU==------\n");
        wprintf(L"\tStart\n");
        wprintf(L"\tLoad\n");
        wprintf(L"\tExit\n");
        wprintf(L"------=============------\n");

        // Check for error if it's 3 then print the save error and then return 0
        saving_error = error_check(saving_error);

        fgets(os_input, sizeof(os_input), stdin);

        to_upper(os_input);

        // If it is start then break and go to the main, then start.
        if(strcmp(os_input, "START\n") == 0) {
            break;
        }
        // If it is load.
        else if(strcmp(os_input, "LOAD\n") == 0) {

            // read the loaded board.
            // return the value from the function into saving_error
            saving_error = load_board(board);

            // if it is not 3 then no error, break and go to the main, then start.
            if(saving_error != 3) {break;}
        }
        // If it is exit, close the game.
        else if(strcmp(os_input, "EXIT\n") == 0) {
            wprintf(L"SYSTEM: Exiting...\n");
            exit(0);
        }
        // If it is invalid input.
        else {
            saving_error = 1;
            cleaning_buffer(os_input);
        }

    }

}