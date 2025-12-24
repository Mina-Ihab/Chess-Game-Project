#ifndef MOVE_H
#define MOVE_H

#include <wchar.h>

extern int castle1,castle2,castle3,castle4,pessant, castle_change[4]; 
void movement(int srcRow, int srcCol, int destRow, int destCol, wchar_t** board, 
              int player, int *error, wchar_t* Wteam, wchar_t* Bteam, int* Wdead, 
              int* Bdead, wchar_t*** memory_board, int* Maxslot, int slot, int* setting_array);

#endif