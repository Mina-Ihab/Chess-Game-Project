#ifndef MOVE_H
#define MOVE_H

#include <wchar.h>

void movement(int srcRow, int srcCol, int destRow, int destCol, wchar_t** board, int team, int *error, wchar_t* Wteam, wchar_t* Bteam, int* dead);

void rook(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error);
void wPawn(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error);
void bPawn(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error);
void bishop(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error);
void king(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error);
void queen(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error);
void knight(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error);

#endif