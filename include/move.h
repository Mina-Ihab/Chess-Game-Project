#ifndef MOVE_H
#define MOVE_H

#include <wchar.h>
#include <stdbool.h>

extern int castle1,castle2,castle3,castle4,pessant, changes[5]; 
void movement(int srcRow, int srcCol, int destRow, int destCol, wchar_t** board, int player, int *error, wchar_t* Wteam, wchar_t* Bteam, int* Wdead, int* Bdead, wchar_t*** memory_board, int* Maxslot, int slot);
bool isPlaceattacked(int destrow, int destcol, wchar_t** board, int *error, int player, wchar_t* Wteam, wchar_t* Bteam, int* Wdead, int* Bdead, wchar_t*** memory_board, int* Maxslot, int saveSlot);
void dealloction(wchar_t** twoDim, wchar_t*** threeDim);
#endif