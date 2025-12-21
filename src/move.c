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
        case L'♔':// White King
            king(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♚':// Black King
            king(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♕':// White Queen
            queen(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♛':// Black Queen
            queen(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♖':// White Rook
            rook(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♜':// Black Rook
            rook(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♗':// White Bishop
            bishop(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♝':// Black Bishop
            bishop(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♘':// White knight
            knight(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♞':// Black Knight
            knight(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♙':// White Pawn
            wPawn(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
        case L'♟':// Black Pawn
            bPawn(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, &error);
            break;
    }
}

void rook(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){
    if(moveRow!=0&&moveCol!=0 || moveRow==0&&moveCol==0){*error = 5; return;}
    int decision=2;//imposiiple case
    if(moveCol>0){
        for(int i=1; i<moveCol; i++){
            if(!(board[beginRow][i+beginCol]==L'□' || board[beginRow][i+beginCol]==L'■')){*error = 5; return;}
            decision=0;
        }
    }//go right
    if(moveRow>0){
        for(int i=1; i<moveRow; i++){
            if(!(board[i+beginRow][beginCol]==L'□' || board[i+beginRow][beginCol]==L'■')){*error = 5; return;}
            decision=1;
        }
    }//up
    if(moveCol<0){
        for(int i=beginCol-1; i>beginCol+moveCol; i--){
            if(!(board[beginRow][i]==L'□' || board[beginRow][i]==L'■')){*error = 5; return;}
            decision=0;
        }
    }//left
    if(moveRow<0){
        for(int i=beginRow-1; i>beginRow+moveRow; i--){
            if(!(board[i][beginCol]==L'□' || board[i][beginCol]==L'■')){*error = 5; return;}
            decision=1;
        }
    }//down
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
