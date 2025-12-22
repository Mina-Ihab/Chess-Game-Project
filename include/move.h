#ifndef MOVE_H
#define MOVE_H

#include <wchar.h>

void movement(int srcRow, int srcCol, int destRow, int destCol, wchar_t** board, int player, int *error, wchar_t* Wteam, wchar_t* Bteam, int* Wdead, int* Bdead);

#endif