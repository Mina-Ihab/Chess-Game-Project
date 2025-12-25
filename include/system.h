#ifndef SYSTEM_H
#define SYSTEM_H

#include <wchar.h>

extern int saveSlot, max_slot;
void start(wchar_t **board);
void main_menu(wchar_t **board);
void undo_move(wchar_t*** memory_board, wchar_t** board, int* error, int* Wdead, int*Bdead, int update);
int save_move(wchar_t*** memory_board, wchar_t** board, int* Wdead, int* Bdead);
void redo_move(wchar_t*** memory_board, wchar_t** board, int* error, int* Wdead, int* Bdead);

#endif