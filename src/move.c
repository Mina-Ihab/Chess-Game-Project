#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#include "../include/board.h"
#include "../include/move.h"

int castle1=1,castle2=1,castle3=1,castle4=1,pessant=0;//flags special moves

void pormotion(int player, int* error, wchar_t** board, int col){

    char piece;
    printf("Choose the piece to pormote into (capital letter for white and samll for black):");
    scanf("%c", &piece);
    if(player==1){
        switch(piece){
            case 'Q':// Black Queen
                board[0][col]=L'♛';
                break;
            case 'R':// Black Rook
                board[0][col]=L'♜';
                break;
            case 'B':// Black Bishop
                board[0][col]=L'♝';
                break;
            case 'N':// Black Knight
                board[0][col]=L'♞';
                break;
            default:
                *error = 1;
                break;
        }
    }// White pieces
    if(player==0){
        switch(piece){
            case 'q':// White Queen
                board[7][col]=L'♕';
                break;
            case 'r':// White Rook
                board[7][col]=L'♖';
                break;
            case 'b':// White Bishop
                board[7][col]=L'♗';
                break;
            case 'n':// White Knight
                board[7][col]=L'♘';
                break;
            default:
                *error = 1;
                break;
        }// Black pieces
    }
}
void rook(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){
    if(moveRow!=0&&moveCol!=0 || moveRow==0&&moveCol==0){*error = 5; return;}
    int decision=2;//imposiple case
    if(moveCol>0){
        decision=0;
        for(int i=1; i<moveCol; i++){
            if(!(board[beginRow][i+beginCol]==L'□' || board[beginRow][i+beginCol]==L'■')){*error = 5; return;}
        }
    }//go right
    if(moveRow>0){
        decision=1;
        for(int i=1; i<moveRow; i++){
            if(!(board[i+beginRow][beginCol]==L'□' || board[i+beginRow][beginCol]==L'■')){*error = 5; return;}
        }
    }//up
    if(moveCol<0){
        decision=0;
        for(int i=beginCol-1; i>beginCol+moveCol; i--){
            if(!(board[beginRow][i]==L'□' || board[beginRow][i]==L'■')){*error = 5; return;}
        }
    }//left
    if(moveRow<0){
        decision=1;
        for(int i=beginRow-1; i>beginRow+moveRow; i--){
            if(!(board[i][beginCol]==L'□' || board[i][beginCol]==L'■')){*error = 5; return;}     
        }
    }//down
    if(decision==0){
        for(int i=1; i<6; i++){
            if(board[beginRow][beginCol+moveCol]==team[i]){
                if(beginRow==0&&beginCol==0) castle1=0;
                if(beginRow==0&&beginCol==7) castle2=0;
                if(beginRow==7&&beginCol==0) castle3=0;
                if(beginRow==7&&beginCol==7) castle4=0;
                board[beginRow][beginCol+moveCol]=board[beginRow][beginCol];
                dead[i-1]++;
                board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
                pessant=0;
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
            pessant=0;
            return;
        }
        else{*error = 5; return;}
    }
    else if(decision==1){
        for(int i=1; i<6; i++){
            if(board[beginRow+moveRow][beginCol]==team[i]){
                if(beginRow==0&&beginCol==0) castle1=0;
                if(beginRow==0&&beginCol==7) castle2=0;
                if(beginRow==7&&beginCol==0) castle3=0;
                if(beginRow==7&&beginCol==7) castle4=0;
                board[beginRow+moveRow][beginCol]=board[beginRow][beginCol];
                dead[i-1]++;
                board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
                pessant=0;
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
            pessant=0;
            return;
        }
        else{*error = 5; return;}
    }
}
void pawn(int beginRow, int beginCol, int destrow, int destcol, int moveRow, int moveCol, wchar_t** board, wchar_t* oppteam, int* dead, int *error, int player){
    if(moveCol>1 || moveCol<-1 || moveRow>2 || moveRow<-2 || moveRow==0){*error = 5; return;}
    if(moveRow==-2&&beginRow!=6){*error = 5; return;}
    if(moveRow==2&&beginRow!=1){*error = 5; return;}
    if(player==1&&moveRow<0 || player==0&&moveRow>0){*error = 5; return;}// player 1=white moves +, player 0=black moves -
    if(moveRow==2&&!(board[beginRow+1][beginCol]==L'□' || board[beginRow+1][beginCol]==L'■')){*error = 5; return;}
    if(moveRow==-2&&!(board[beginRow-1][beginCol]==L'□' || board[beginRow-1][beginCol]==L'■')){*error = 5; return;}
    if(pessant&&moveCol){
        int target=(player==1)?L'♙': L'♟';//opposite team
        if(target==board[beginRow][beginCol+moveCol]){
            board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
            board[beginRow][beginCol+moveCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            dead[5]++;
            pessant=0;
            return;
        }
    }
    if (moveCol && (moveRow == 1 || moveRow == -1)){
        for(int i=1; i<6; i++){
            if(board[beginRow+moveRow][beginCol+moveCol]==oppteam[i]){
                board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
                dead[i-1]++;
                board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
                pessant=0;
                if(destrow==7 || destrow==0)pormotion(player,error,board,destcol);
                return;
            }
        }
        *error = 5; return;//if didnt return then mean no piece then invalid move
    }    
    if ((board[beginRow+moveRow][beginCol+moveCol]==L'□' || board[beginRow+moveRow][beginCol+moveCol]==L'■')){
        board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
        board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
        pessant=(moveRow==2 || moveRow==-2)?1:0;
        if(destrow==7 || destrow==0)pormotion(player,error,board,destcol);
        return;
    }
    else{*error = 5; return;}
}
void bishop(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){
    int negative1=0,negative2=0;
    if(moveRow<0){moveRow=-moveRow;negative1=1;}
    if(moveCol<0){moveCol=-moveCol;negative2=1;}//handle negative for compare
    if(moveRow!=moveCol || moveRow==0&&moveCol==0){*error = 5; return;}
    if(negative1==1)moveRow=-moveRow;
    if(negative2==1)moveCol=-moveCol;//return negative
    if(moveCol>0&&moveRow>0){
        for(int i=1; i<moveRow; i++){
            for(int j=1; j<moveCol; j++){
                if(i!=j)continue;
                if(!(board[i+beginRow][j+beginCol]==L'□' || board[i+beginRow][j+beginCol]==L'■')){*error = 5; return;}
            }
        }
    }//go up right
    if(moveCol<0&&moveRow>0){
        for(int i=1; i<moveRow; i++){
            for(int j=beginCol-1; j>moveCol+beginCol; j--){
                if(i+j!=beginCol)continue;
                if(!(board[i+beginRow][j]==L'□' || board[i+beginRow][j]==L'■')){*error = 5; return;}
            }
        }
    }//up left
    if(moveCol>0&&moveRow<0){
        for(int i=beginRow-1; i>beginRow+moveRow; i--){
            for(int j=1; j<moveCol; j++){
                if(i+j!=beginCol+beginRow)continue;
                if(!(board[i][j+beginCol]==L'□' || board[i][j+beginCol]==L'■')){*error = 5; return;}
            }
        }
    }//down right
    if(moveCol<0&&moveRow<0){
        for(int i=beginRow-1; i>beginRow+moveRow; i--){
            for(int j=beginCol-1; j>moveCol+beginCol; j--){
                if(i-j!=beginRow-beginCol)continue;
                if(!(board[i][j]==L'□' || board[i][j]==L'■')){*error = 5; return;}
            }
        }
    }//down left
    for(int i=1; i<6; i++){
        if(board[beginRow+moveRow][beginCol+moveCol]==team[i]){
            board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
            dead[i-1]++;
            board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            pessant=0;
            return;
        }
    }
    if((board[beginRow+moveRow][beginCol+moveCol]==L'□' || board[beginRow+moveRow][beginCol+moveCol]==L'■')){
        board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
        board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
        pessant=0;
        return;
    }
    else{*error = 5; return;}
}
void knight(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){
    if(moveCol>2 || moveCol<-2 || moveRow>2 || moveRow<-2 || moveRow==0 || moveCol==0){*error = 5; return;}
    int negative1=0,negative2=0;
    if(moveRow<0){moveRow=-moveRow;negative1=1;}
    if(moveCol<0){moveCol=-moveCol;negative2=1;}//handle negative for compare
    if(moveCol==2&&moveRow!=1){*error = 5; return;}
    if(moveCol!=2&&moveRow==1){*error = 5; return;}
    if(negative1==1)moveRow=-moveRow;
    if(negative2==1)moveCol=-moveCol;//return negative
    for(int i=1; i<6; i++){
        if(board[beginRow+moveRow][beginCol+moveCol]==team[i]){
            board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
            dead[i-1]++;
            board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            pessant=0;
            return;
        }
    }
    if((board[beginRow+moveRow][beginCol+moveCol]==L'□' || board[beginRow+moveRow][beginCol+moveCol]==L'■')){
        board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
        board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
        pessant=0;
        return;
    }
    else{*error = 5; return;}
}
void Wking(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){

}
void Bking(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){

}
void queen(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){

}

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
            Wking(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);//error no need to * or & as it was from caller int* error
            break;
        case L'♚':// Black King
            Bking(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♕':// White Queen
            queen(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♛':// Black Queen
            queen(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♖':// White Rook
            rook(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♜':// Black Rook
            rook(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♗':// White Bishop
            bishop(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♝':// Black Bishop
            bishop(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♘':// White knight
            knight(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♞':// Black Knight
            knight(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♙':// White Pawn
            pawn(srcRow, srcCol, destRow, destCol, moveRow, moveCol, board, oppteam, oppdead, error, player);
            break;
        case L'♟':// Black Pawn
            pawn(srcRow, srcCol, destRow, destCol, moveRow, moveCol, board, oppteam, oppdead, error, player);
            break;
    }
}