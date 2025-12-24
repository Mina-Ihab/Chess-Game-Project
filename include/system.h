#ifndef SYSTEM_H
#define SYSTEM_H

#include <wchar.h>

void start(wchar_t **board);
void main_menu(wchar_t **board);
void undo_move(wchar_t*** memory_board, wchar_t** board, int* error, int* max, int update, int* setting_array);
int save_move(wchar_t*** memory_board, wchar_t** board);

#endif