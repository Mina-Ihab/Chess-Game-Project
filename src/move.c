#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#include "../include/board.h"
#include "../include/move.h"

int castle1=1,castle2=1,castle3=1,castle4=1;

void movement(int srcRow, int srcCol, int destRow, int destCol, wchar_t** board, 
            int player, int *error, wchar_t* Wteam, wchar_t* Bteam, int* Wdead, int* Bdead){
    wchar_t piece=board[srcRow][srcCol];
    wchar_t* oppteam=(player==1)?Bteam:Wteam;//to check if the team of opposite side
    int* oppdead=(player==1)?Bdead:Wdead;

    if(player==1){
        int flag=1;
        for(int i=0; i<6; i++){//6 different pieces type
            if(piece==oppteam[i]){flag=0;break;}
        }
        if(flag==0){*error = 4; return;}
    }
    else{
        int flag=1;
        for(int i=0; i<6; i++){
            if(piece==oppteam[i]){flag=0;break;}
        }
        if(flag==0){*error = 4; return;}
    }
    int moveRow=destRow-srcRow;
    int moveCol=destCol-srcCol;
    switch(piece){
        case L'♔':
            king(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♚': // King
            king(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♕':
            queen(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♛': // Queen
            queen(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♖':
            rook(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♜': // Rook
            rook(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♗':
            bishop(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♝': // Bishop
            bishop(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♘':
            knight(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♞': // Knight
            knight(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♙': // White Pawn
            wPawn(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♟': // Black Pawn
            bPawn(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
    }
}
//*didnt make the castle
void rook(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){
    if(moveRow!=0&&moveCol!=0 || moveRow==0&&moveCol==0){*error = 5; return;}
    int decision;
    for(int i=beginCol; i<moveCol-1; i++){
        if(!(board[beginRow][i]==L'□'&&board[beginRow][i]==L'■')){*error = 5; return;}
        decision=0;
    }
    for(int i=beginRow; i<moveRow-1; i++){
        if(!(board[i][beginCol]==L'□'&&board[i][beginCol]==L'■')){*error = 5; return;}
        decision=1;
    }
    if(decision==0){
        for(int i=0; i<6; i++){
            if(board[beginRow][beginCol+moveCol]==team[i]){
                if(beginRow==0&&beginCol==0) castle1=0;
                if(beginRow==0&&beginCol==7) castle2=0;
                if(beginRow==7&&beginCol==0) castle3=0;
                if(beginRow==7&&beginCol==7) castle4=0;
                board[beginRow][beginCol+moveCol]=board[beginRow][beginCol];
                dead[i]++;
                board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
                return;
            }
        }
        if((board[beginRow][beginCol+moveCol]==L'□' || board[beginRow][beginCol+moveCol]==L'■')){
            if(beginRow==0&&beginCol==0) castle1=0;
            if(beginRow==0&&beginCol==7) castle2=0;
            if(beginRow==7&&beginCol==0) castle3=0;
            if(beginRow==7&&beginCol==7) castle4=0;
            board[beginRow][beginCol+moveCol]=board[beginRow][beginCol];
            board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            return;
        }
        else{*error = 5; return;}
    }
    else if(decision==1){
        for(int i=0; i<6; i++){
            if(board[beginRow+moveRow][beginCol]==team[i]){
                if(beginRow==0&&beginCol==0) castle1=0;
                if(beginRow==0&&beginCol==7) castle2=0;
                if(beginRow==7&&beginCol==0) castle3=0;
                if(beginRow==7&&beginCol==7) castle4=0;
                board[beginRow+moveRow][beginCol]=board[beginRow][beginCol];
                dead[i]++;
                board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
                return;
            }
        }
        if((board[beginRow+moveRow][beginCol]==L'□' || board[beginRow+moveRow][beginCol]==L'■')){
            if(beginRow==0&&beginCol==0) castle1=0;
            if(beginRow==0&&beginCol==7) castle2=0;
            if(beginRow==7&&beginCol==0) castle3=0;
            if(beginRow==7&&beginCol==7) castle4=0;
            board[beginRow+moveRow][beginCol]=board[beginRow][beginCol];
            board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            return;
        }
        else{*error = 5; return;}
    }
}
void wPawn(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){

}
void bPawn(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){

}
void bishop(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){

}
void knight(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){

}
void king(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){

}
void queen(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){

}
