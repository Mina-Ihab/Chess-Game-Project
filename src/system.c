#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>

#include "../include/board.h"


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
int error_check(int error) {

    // Invalid Input Error 1
    if(error == 1) {
        wprintf(L"ERROR: Invalid input. Please try again.\n"); 
        error = 0;
    }
    // Empty Place Error 2
    else if(error == 2) {
        wprintf(L"ERROR: You selected an empty place. Choose a valid one.\n");
        error = 0;
    }
    // Save Error 3
    else if(error == 3) {
        wprintf(L"SYSTEM: You don't have any saved games yet.\n");
        error = 0;
    }
    else if(error == 4) {
        wprintf(L"ERROR: This is not your piece.\n");
        error = 0;
    }
    else if(error == 5) {
        wprintf(L"ERROR: Invalid movement.\n");
        error = 0;
    }

    return error;
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

// Start the game
void start(wchar_t **board) {

    // 1 = White Team
    // 0 = Black Team
    int switching_team = 1;

    int error = 0;
    int save = 0;
    char input[10];

    //we can remove the 1 and make it, if the game is not end 
    while(1) {

        if(switching_team) {

            //Clear Terminal before each print
            clear_terminal();

            //print the board
            print_board(board);

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
            pawnMove(sel_row,sel_col,dest_row,dest_col,board,switching_team, &error);
            if(error != 0) continue;

            //switch the team
            switching_team = 0;

        }
        else {
            
            //Clear Terminal before each print
            clear_terminal();

            //print the board
            print_board(board);

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
            pawnMove(sel_row,sel_col,dest_row,dest_col,board,switching_team, &error);
            if(error != 0) continue;

            //switch the team
            switching_team = 1;

        }

    }

}

// Main Menu
void main_menu(wchar_t **board) {

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
            exit(0);
        }
        // If it is invalid input.
        else {
            saving_error = 1;
            cleaning_buffer(os_input);
        }

    }

}