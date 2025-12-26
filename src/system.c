#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>

#include "../include/board.h"
#include "../include/move.h"


int saveSlot = -1;
int max_slot = -1;

// 1 = White Team
// 0 = Black Team
int team = 1;

int Wdead[6]={0,0,0,0,0,0}, Bdead[6]={0,0,0,0,0,0};

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
    else if(msg_id == 9) {
        wprintf(L"SYSTEM: Check\n");
    }

    return 0;
}

// Cleaning the buffer.
// it only work when there is no '\n'.
void cleaning_buffer(char* string) {

    //If there is no end of line (that's mean there is a buffer)
    if(strchr(string, '\n') == NULL) {
        int ch;
        // eof --> to make the while exit when the character is new line or end of input
        while((ch = getchar()) != '\n' && ch != EOF);
    }

}

// Write the board in a file 'save.txt'
void save_board(wchar_t **board) {

    // open the file
    FILE *saveBoardFile = fopen("./bin/save.txt", "w");
    FILE *saveDataFile = fopen("./bin/saveData.txt", "w");

    char data[18];

    data[0] = team + '0';
    data[1] = castle1 + '0';
    data[2] = castle2 + '0';
    data[3] = castle3 + '0';
    data[4] = castle4 + '0';
    data[5] = pessant + '0';

    for(int i = 0; i < 6; i++)
        data[i+6] = Wdead[i] + '0'; // 6 7 8 9 10 11

    for(int i = 0; i < 6; i++)
        data[i+12] = Bdead[i] + '0'; // 12 13 14 15 16 17

    for(int i = 0; i < 8; i++) {

        // write row by row each element of size wchar_t and they are 8 element
        fwrite(board[i], sizeof(wchar_t), 8, saveBoardFile);

    }

    fwrite(data, sizeof(char), 18, saveDataFile);

    // close the file
    fclose(saveBoardFile);
    fclose(saveDataFile);

}

// load the board from the file 'save.txt'
int load_board(wchar_t **board) {

    // open the file
    FILE *saveBoardFile = fopen("./bin/save.txt", "r");
    FILE *saveDataFile = fopen("./bin/saveData.txt", "r");

    char data[18];

    // if there is no file
    if(saveBoardFile == NULL) {
        // it will return 3 to the error variable which will make the loop continue and send error.
        return 3;
    }
    
    for(int i = 0; i < 8; i++) {

        // read row by row each element of size wchar_t and they are 8 element
        fread(board[i], sizeof(wchar_t), 8, saveBoardFile);

    }

    fread(data, sizeof(char), 18, saveDataFile);

    team = data[0] - '0';
    castle1 = data[1] - '0';
    castle2 = data[2] - '0';
    castle3 = data[3] - '0';
    castle4 = data[4] - '0';
    pessant = data[5] - '0';

    for(int i = 0; i < 6; i++)
        Wdead[i] = data[i+6] - '0'; // 6 7 8 9 10 11

    for(int i = 0; i < 6; i++)
        Bdead[i] = data[i+12] - '0';

    // close the file
    fclose(saveBoardFile);
    fclose(saveDataFile);

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
        board_memory[i] = (wchar_t **) calloc(11, sizeof(wchar_t*));
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

        for(int j = 8; j < 11; j++) {
            board_memory[i][j] = (wchar_t *) calloc(6, sizeof(wchar_t));
            if(board_memory[i][j] == NULL) {
                printf("ERROR: There is no free space to create board memory!");
                exit(0);
            }
        }
    }

    return board_memory;
}

// Save the board for UNDO/REDO
int save_move(wchar_t*** memory_board, wchar_t** board, int* Wdead, int* Bdead) {

    saveSlot++;

    for(int r = 0; r < 8; r++)
        for(int c = 0; c < 8; c++)
            memory_board[saveSlot][r][c] = board[r][c];

    for(int r = 8; r < 9; r++)
        for(int c = 0; c < 6; c++)
            memory_board[saveSlot][r][c] = Wdead[c];
    
    for(int r = 9; r < 10; r++)
        for(int c = 0; c < 6; c++)
            memory_board[saveSlot][r][c] = Bdead[c];

    memory_board[saveSlot][10][0] = castle1;
    memory_board[saveSlot][10][1] = castle2;
    memory_board[saveSlot][10][2] = castle3;
    memory_board[saveSlot][10][3] = castle4;
    memory_board[saveSlot][10][4] = pessant;

}

// UNDO Move
void undo_move(wchar_t*** memory_board, wchar_t** board, int* error, int* Wdead, int*Bdead, int update) {

    if(saveSlot < 1){*error = 7;return;}
    saveSlot--;
    for(int r = 0; r < 8; r++)
        for(int c = 0; c < 8; c++)
            board[r][c] = memory_board[saveSlot][r][c];

    for(int r = 8; r < 9; r++)
        for(int c = 0; c < 6; c++)
            Wdead[c] = memory_board[saveSlot][r][c];

    for(int r = 9; r < 10; r++)
        for(int c = 0; c < 6; c++)
            Bdead[c] = memory_board[saveSlot][r][c];
    
    castle1 = memory_board[saveSlot][10][0];
    castle2 = memory_board[saveSlot][10][1];
    castle3 = memory_board[saveSlot][10][2];
    castle4 = memory_board[saveSlot][10][3];
    pessant = memory_board[saveSlot][10][4];

    if(update == 1) saveSlot = max_slot;

}

// REDO Move
void redo_move(wchar_t*** memory_board, wchar_t** board, int* error, int* Wdead, int* Bdead) {

    saveSlot++;

    if(saveSlot > max_slot) {
        saveSlot--;
        *error = 8;
        return;
    }

    for(int r = 0; r < 8; r++)
        for(int c = 0; c < 8; c++)
            board[r][c] = memory_board[saveSlot][r][c];

    for(int r = 8; r < 9; r++)
        for(int c = 0; c < 6; c++)
            Wdead[c] = memory_board[saveSlot][r][c];

    for(int r = 9; r < 10; r++)
        for(int c = 0; c < 6; c++)
            Bdead[c] = memory_board[saveSlot][r][c];
    
    castle1 = memory_board[saveSlot][10][0];
    castle2 = memory_board[saveSlot][10][1];
    castle3 = memory_board[saveSlot][10][2];
    castle4 = memory_board[saveSlot][10][3];
    pessant = memory_board[saveSlot][10][4];

}

// print the array of dead
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

// to check the decision
int decision_check(int decision, wchar_t ** board) {

    if(decision == 1) {
        wprintf(L"SYSTEM: Game saved successfully!\n");
        save_board(board);
    }
    else if (decision == 2) {
        wprintf(L"---==CMD==---\n");
        wprintf(L"Undo - to go the previous move.\n");
        wprintf(L"Redo - to go the next move (if available).\n");
        wprintf(L"Save - to save your current game and data.\n");
        wprintf(L"Quit - to leave from the game.\n");
        wprintf(L"---=======---\n");
    }

    return 0;
}

int lenght_chec(char* string) {

    // Get the length of the string
    int lenght = strlen(string);
    // replace last character by \0
    if(lenght > 0 && string[lenght-1] == '\n') {
        string[lenght-1] ='\0';
        lenght--;
    }
    // if it is not 4 then return 1 to the error
    if(strlen(string) > 4) {
        wprintf(L"errror\n");
        return 1;
    }
    return 0;
}

void gamePlaySystem(wchar_t*** memory_board, wchar_t** board, wchar_t* Bteam, wchar_t* Wteam, wchar_t* Bdead, wchar_t* Wdead,
                    int* error, int* decision, int* isUndo, int* isRedo,
                    int* team, char* input, int size, int* s_r, char* s_c, int* d_r, char* d_c, 
                    int* s_row, int* s_col, int* d_row, int* d_col) {

    if(*team == 1) {wprintf(L"White's turn (format: B1C3, type 'cmd' for commands):\n--> ");}
    else {wprintf(L"Black's turn (format: B1C3, type 'cmd' for commands):\n--> ");}
        
    fgets(input, size, stdin);

    //Cleaning Buffer
    cleaning_buffer(input);

    // Make Sure input is 4 Letters
    *error = lenght_chec(input);
    if(*error == 1) return;

    //Make the letter UpperCase
    input[0] = toupper(input[0]);
    input[2] = toupper(input[2]);
    
    // check validity of input
    if(sscanf(input, "%c %d %c %d", s_c, s_r, d_c, d_r) == 4) {
        
        *s_col = *s_c - 'A';
        *d_col = *d_c - 'A';
        *s_row = *s_r - 1;
        *d_row = *d_r - 1;

    }
    else {
        // if it's not a game input
        input[1] = toupper(input[1]);
        input[3] = toupper(input[3]);

        // Check if it is a saving input
        if(strcmp(input, "CMD") == 0) {
            *decision = 2;
            return;
        }
        else if(strcmp(input, "SAVE") == 0) {
            *decision = 1; 
            return;
        }
        // UNDO
        else if(strcmp(input, "UNDO") == 0) {

            undo_move(memory_board, board, error, Wdead, Bdead, 0);
            if(*error == 0) {
                if(*team == 1) *team = 0;
                else *team = 1;
            }
            *isUndo = 1;
            return;

        }
        // REDO
        else if(strcmp(input, "REDO") == 0) {

            redo_move(memory_board, board, error, Wdead, Bdead);
            if(*error == 0) {
                if(*team == 1) *team = 0;
                else *team = 1;
            }
            *isRedo = 1;
            return;

        }
        // QUIT from the game
        else if(strcmp(input, "QUIT") == 0) {

            dealloction(board, memory_board);
            wprintf(L"Exiting...\n");
            exit(0);

        }
        // Else this is an error
        else {

            *error = 1;
            return;

        }
    }

    //Check if its in the board
    int isOut = (*s_col < 0 || *s_col > 7) || (*d_col < 0 || *d_col > 7) || (*s_row < 0 || *s_row > 7) || (*d_row < 0 || *d_row > 7);
    if(isOut) {
        *error = 1;
        return;
    }


    //Check for empty place
    int isEmpty = (board[*s_row][*s_col] == L'□' || board[*s_row][*s_col] == L'■');
    if(isEmpty) {
        *error = 2;
        return;
    }

}

// Start the game
void start(wchar_t **board, wchar_t*** memory_board) {

    int error = 0;
    int decision = 0;
    int lenght;
    char input[6];//4 inputs + \n +\0
    int isUndo = 0;
    int isRedo = 0;

    //we made this colors to be understandable as the white seams white in vscode darkmode
    wchar_t white_team[6]={L'♚', L'♛', L'♜', L'♝', L'♞', L'♟'};
    wchar_t black_team[6]={L'♔', L'♕', L'♖', L'♗', L'♘', L'♙'};

    // To save the first slot
    save_move(memory_board, board, Wdead, Bdead);
    max_slot = saveSlot;

    //Declare Co-ordination
        //For scaning:
            char s_col, d_col;
            int s_row, d_row;
        //Real Co-ordination:
            int sel_col, sel_row, dest_row, dest_col;

    //we can remove the 1 and make it, if the game is not end 
    while(1) {

        if(team) {

            //Clear Terminal before each print
            clear_terminal();

            //print the board
            print_board(board);
            print_dead(black_team, white_team, Wdead, Bdead);

            //if the loop is repeated due to an error
            error = error_check(error);
            
            //if the loop is repeated due to save
            decision = decision_check(decision, board);
            
            gamePlaySystem(memory_board, board, black_team, white_team, Bdead, Wdead, &error, &decision, &isUndo, &isRedo, &team, input, sizeof(input), &s_row, &s_col, &d_row, &d_col, &sel_row, &sel_col, &dest_row, &dest_col);
            if(error != 0) continue;
            if(decision != 0) continue;
            if(isUndo) {isUndo = 0; continue;}
            if(isRedo) {isRedo = 0; continue;}

            //Move the piece
            movement(sel_row, sel_col, dest_row, dest_col, board, team,
                     &error, white_team, black_team, Wdead, Bdead, memory_board);
            
            // Handle some erros related to the movement
            if(error != 0 && error != 9) {
                if(error==6){save_move(memory_board, board, Wdead, Bdead);max_slot = saveSlot;max_slot--;undo_move(memory_board, board, &error, Wdead, Bdead, 1);}
                continue;
            }
            else if (error == 9) {
                save_move(memory_board, board, Wdead, Bdead);
                max_slot = saveSlot;
                max_slot--;
                undo_move(memory_board, board, &error, Wdead, Bdead, 1);
            }

            //switch the team
            team = 0;

        }
        else {
            
            //Clear Terminal before each print
            clear_terminal();

            //print the board
            print_board(board);
            print_dead(black_team, white_team, Wdead, Bdead);

            //if the loop is repeated due an error
            error = error_check(error);

            //if the loop is repeated due to save
            decision = decision_check(decision, board);

            gamePlaySystem(memory_board, board, black_team, white_team, Bdead, Wdead, &error, &decision, &isUndo, &isRedo, &team, input, sizeof(input), &s_row, &s_col, &d_row, &d_col, &sel_row, &sel_col, &dest_row, &dest_col);
            if(error != 0) continue;
            if(decision != 0) continue;
            if(isUndo) {isUndo = 0; continue;}
            if(isRedo) {isRedo = 0; continue;}


            //Move the piece
            movement(sel_row, sel_col, dest_row, dest_col, board, team,
                     &error, white_team, black_team, Wdead, Bdead, memory_board);

            // Handle some erros related to the movement
            if(error != 0 && error != 9) {
                if(error==6){save_move(memory_board, board, Wdead, Bdead);max_slot = saveSlot;max_slot--;undo_move(memory_board, board, &error, Wdead, Bdead, 1);}
                continue;
            }
            else if (error == 9) {
                save_move(memory_board, board, Wdead, Bdead);
                max_slot = saveSlot;
                max_slot--;
                undo_move(memory_board, board, &error, Wdead, Bdead, 1);
            }

            //switch the team
            team = 1;

        }
        // save the game
        // here we will have dublicate save because there is one in movement if it canMove
        save_move(memory_board, board, Wdead, Bdead);
        // update max slot
        max_slot = saveSlot;
        // remove one from the max slot to remove the dublicate
        max_slot--;
        // undo movement and '1' mean update the slot to be the max slot
        undo_move(memory_board, board, &error, Wdead, Bdead, 1);

    }
}

// Main Menu
void main_menu(wchar_t **board, wchar_t *** memoryboard) {

    char os_input[10];
    int saving_error = 0;

    while(1) {

        clear_terminal();

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
            dealloction(board, memoryboard);
            exit(0);
        }
        // If it is invalid input.
        else {
            saving_error = 1;
            cleaning_buffer(os_input);
        }

    }

}