#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#include "../include/board.h"
#include "../include/move.h"
#include "../include/system.h"

// to return castle when undo and redo
int castle1=1,castle2=1,castle3=1,castle4=1,pessant=0; //flags special moves

void pormotion(int player, int* error, wchar_t** board, int col){
    char piece_s[3], piece;
    int error_flag = 0;
    do {
        wprintf(L"Choose the piece to pormote into:");
        fgets(piece_s, sizeof(piece_s), stdin);
        sscanf(piece_s, "%c", &piece);
        if(player==1){
            piece = toupper(piece);
            switch(piece){
                case 'Q':// white Queen
                    board[7][col]=L'♛';
                    error_flag = 0;
                    break;
                case 'R':// white Rook
                    board[7][col]=L'♜';
                    error_flag = 0;
                    break;
                case 'B':// white Bishop
                    board[7][col]=L'♝';
                    error_flag = 0;
                    break;
                case 'N':// white Knight
                    board[7][col]=L'♞';
                    error_flag = 0;
                    break;
                default:
                    error_flag = 1;
                    break;
            }
        }// White pieces
        if(player==0){
            piece = tolower(piece);
            switch(piece){
                case 'q':// black Queen
                    board[0][col]=L'♕';
                    error_flag = 0;
                    break;
                case 'r':// black Rook
                    board[0][col]=L'♖';
                    error_flag = 0;
                    break;
                case 'b':// black Bishop
                    board[0][col]=L'♗';
                    error_flag = 0;
                    break;
                case 'n':// black Knight
                    board[0][col]=L'♘';
                    error_flag = 0;
                    break;
                default:
                    error_flag = 1;
                    break;
            }// Black pieces
        }
    }while(error_flag!=0);
}
bool rook(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){
    if(moveRow!=0&&moveCol!=0 || moveRow==0&&moveCol==0){*error = 5; return false;}
    int decision=2;//imposiple case
    if(moveCol>0){
        decision=0;
        for(int i=1; i<moveCol; i++){
            if(!(board[beginRow][i+beginCol]==L'□' || board[beginRow][i+beginCol]==L'■')){*error = 5; return false;}
        }
    }//go right
    if(moveRow>0){
        decision=1;
        for(int i=1; i<moveRow; i++){
            if(!(board[i+beginRow][beginCol]==L'□' || board[i+beginRow][beginCol]==L'■')){*error = 5; return false;}
        }
    }//up
    if(moveCol<0){
        decision=0;
        for(int i=beginCol-1; i>beginCol+moveCol; i--){
            if(!(board[beginRow][i]==L'□' || board[beginRow][i]==L'■')){*error = 5; return false;}
        }
    }//left
    if(moveRow<0){
        decision=1;
        for(int i=beginRow-1; i>beginRow+moveRow; i--){
            if(!(board[i][beginCol]==L'□' || board[i][beginCol]==L'■')){*error = 5; return false;}     
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
                pessant=0;
                return true;
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
            return true;
        }
        else{*error = 5; return false;}
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
                pessant=0;
                return true;
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
            return true;
        }
        else{*error = 5; return false;}
    }
}
bool pawn(int beginRow, int beginCol, int destrow, int destcol, int moveRow, int moveCol, wchar_t** board, wchar_t* oppteam, int* dead, int *error, int player){
    if(moveCol>1 || moveCol<-1 || moveRow>2 || moveRow<-2 || moveRow==0){*error = 5; return false;}
    if(moveRow==-2&&beginRow!=6){*error = 5; return false;}
    if(moveRow==2&&beginRow!=1){*error = 5; return false;}
    if(player==1&&moveRow<0 || player==0&&moveRow>0){*error = 5; return false;}// player 1=white moves +, player 0=black moves -
    if(moveRow==2&&!(board[beginRow+1][beginCol]==L'□' || board[beginRow+1][beginCol]==L'■')){*error = 5; return false;}
    if(moveRow==-2&&!(board[beginRow-1][beginCol]==L'□' || board[beginRow-1][beginCol]==L'■')){*error = 5; return false;}
    if(pessant&&moveCol){
        int target=(player==1)?L'♙': L'♟';//opposite team
        if(target==board[beginRow][beginCol+moveCol]){
            board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
            board[beginRow][beginCol+moveCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            dead[5]++;
            pessant=0;
            return true;
        }
    }
    if (moveCol && (moveRow == 1 || moveRow == -1)){
        for(int i=0; i<6; i++){
            if(board[beginRow+moveRow][beginCol+moveCol]==oppteam[i]){
                board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
                dead[i]++;
                board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
                pessant=0;
                return true;
            }
        }
        *error = 5; return false;//if didnt return then mean no piece then invalid move
    }    
    if ((board[beginRow+moveRow][beginCol]==L'□' || board[beginRow+moveRow][beginCol]==L'■') && moveCol == 0){
        board[beginRow+moveRow][beginCol]=board[beginRow][beginCol];//move head only if cant take
        board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
        pessant=(moveRow==2 || moveRow==-2)?1:0;
        return true;
    }
    else{*error = 5; return false;}
}
bool bishop(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){
    int negative1=0,negative2=0;
    if(moveRow<0){moveRow=-moveRow;negative1=1;}
    if(moveCol<0){moveCol=-moveCol;negative2=1;}//handle negative for compare
    if(moveRow!=moveCol || moveRow==0&&moveCol==0){*error = 5; return false;}
    if(negative1==1)moveRow=-moveRow;
    if(negative2==1)moveCol=-moveCol;//return negative
    
    if(moveCol>0&&moveRow<0){
        for(int i=1; i< moveCol; i++){
            if(!(board[beginRow-i][beginCol+i]==L'□' || board[beginRow-i][beginCol+i]==L'■')){
                *error = 5; 
                return false;
            }
        }
    } // up right
    if(moveCol<0&&moveRow<0){ 
        for(int i=1; i< -(moveRow); i++){
            if(!(board[beginRow-i][beginCol-i]==L'□' || board[beginRow-i][beginCol-i]==L'■')){
                *error = 5; 
                return false;
            }
        }
    } // up left
    if(moveCol>0&&moveRow>0){
        for(int i=1; i< moveRow; i++){
            if(!(board[beginRow+i][beginCol+i]==L'□' || board[beginRow+i][beginCol+i]==L'■')){
                *error = 5; 
                return false;
            }
        }
    } // down right
    if(moveCol<0&&moveRow>0){ 
        for(int i=1; i< moveRow; i++){
            if(!(board[beginRow+i][beginCol-i]==L'□' || board[beginRow+i][beginCol-i]==L'■')){
                *error = 5; 
                return false;
            }
        }
    } // down left 
    for(int i=0; i<6; i++){
        if(board[beginRow+moveRow][beginCol+moveCol]==team[i]){
            board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
            dead[i]++;
            board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            pessant=0;
            return true;
        }
    }
    if((board[beginRow+moveRow][beginCol+moveCol]==L'□' || board[beginRow+moveRow][beginCol+moveCol]==L'■')){
        board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
        board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
        pessant=0;
        return true;
    }
    else{*error = 5; return false;}
}
bool knight(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){
    if(moveCol>2 || moveCol<-2 || moveRow>2 || moveRow<-2 || moveRow==0 || moveCol==0){*error = 5; return false;}
    int negative1=0,negative2=0;
    if(moveRow<0){moveRow=-moveRow;negative1=1;}
    if(moveCol<0){moveCol=-moveCol;negative2=1;}//handle negative for compare
    if(moveCol==2&&moveRow!=1){*error = 5; return false;}
    if(moveCol!=2&&moveRow==1){*error = 5; return false;}
    if(negative1==1)moveRow=-moveRow;
    if(negative2==1)moveCol=-moveCol;//return negative
    for(int i=0; i<6; i++){
        if(board[beginRow+moveRow][beginCol+moveCol]==team[i]){
            board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
            dead[i]++;
            board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            pessant=0;
            return true;
        }
    }
    if((board[beginRow+moveRow][beginCol+moveCol]==L'□' || board[beginRow+moveRow][beginCol+moveCol]==L'■')){
        board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
        board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
        pessant=0;
        return true;
    }
    else{*error = 5; return false;}
}
bool king(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error, int player){
    //castle
    if(player==1){
        if(castle1&&moveCol==2&&moveRow==0){
                for(int i=1; i<moveCol; i++){
                    if(!(board[beginRow][i+beginCol]==L'□' || board[beginRow][i+beginCol]==L'■')){*error = 5; return false;}
                }
            board[beginRow][beginCol+moveCol]=board[beginRow][beginCol];
            board[beginRow][beginCol+1]= L'♜';
            board[beginRow][beginCol]=L'□';
            board[0][7]=L'■';
            castle1=0;castle2=0;
            return true;
        }
        if(castle2&&moveCol==-2&&moveRow==0){
                for(int i=beginCol-1; i>beginCol+moveCol; i--){
                    if(!(board[beginRow][i]==L'□' || board[beginRow][i]==L'■')){*error = 5; return false;}
                }
            board[beginRow][beginCol+moveCol]=board[beginRow][beginCol];
            board[beginRow][beginCol-1]= L'♜';
            board[beginRow][beginCol]=L'□';
            board[0][0]=L'□';
            castle1=0;castle2=0;
            return true;
        }
    }
    else if(player==0){
        if(castle3&&moveCol==2&&moveRow==0){
                for(int i=1; i<moveCol; i++){
                    if(!(board[beginRow][i+beginCol]==L'□' || board[beginRow][i+beginCol]==L'■')){*error = 5; return false;}
                }
            board[beginRow][beginCol+moveCol]=board[beginRow][beginCol];
            board[beginRow][beginCol+1]= L'♖';
            board[beginRow][beginCol]=L'■';
            board[7][7]=L'■';
            castle3=0;castle4=0;
            return true;
        }
        if(castle4&&moveCol==-2&&moveRow==0){
                for(int i=beginCol-1; i>beginCol+moveCol; i--){
                    if(!(board[beginRow][i]==L'□' || board[beginRow][i]==L'■')){*error = 5; return false;}
                }
            board[beginRow][beginCol+moveCol]=board[beginRow][beginCol];
            board[beginRow][beginCol-1]= L'♖';
            board[beginRow][beginCol]=L'■';
            board[7][0]=L'□';
            castle3=0;castle4=0;;
            return true;
        }
    }
    //normal move
    if(moveCol>1 || moveCol<-1 || moveRow>1 || moveRow<-1 || moveRow==0&&moveCol==0){*error = 5; return false;}
    for(int i=0; i<6; i++){
        if(board[beginRow+moveRow][beginCol+moveCol]==team[i]){
            if(board[beginRow][beginCol] == L'♚') {castle1=0;castle2=0;}
            else {castle3=0;castle4=0;}
            board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
            dead[i]++;
            board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
            pessant=0;
            return true;
        }
    }
    if((board[beginRow+moveRow][beginCol+moveCol]==L'□' || board[beginRow+moveRow][beginCol+moveCol]==L'■')){
        if(board[beginRow][beginCol] == L'♚') {castle1=0;castle2=0;}
        else {castle3=0;castle4=0;}
        board[beginRow+moveRow][beginCol+moveCol]=board[beginRow][beginCol];
        board[beginRow][beginCol] = (beginRow+beginCol)%2==0?L'□':L'■';
        pessant=0;
        return true;
    }
    else{*error = 5; return false;}
}
bool queen(int beginRow, int beginCol, int moveRow, int moveCol, wchar_t** board, wchar_t* team, int* dead, int *error){
    int negative1=0,negative2=0;
    if(moveRow<0){moveRow=-moveRow;negative1=1;}
    if(moveCol<0){moveCol=-moveCol;negative2=1;}//handle negative for compare
    int decision=0; //imposible casse
    decision=(moveRow==moveCol)?1:2;
    if(negative1==1)moveRow=-moveRow;
    if(negative2==1)moveCol=-moveCol;//return negative
    if(decision==1){
        if(bishop(beginRow, beginCol, moveRow, moveCol, board, team, dead, error)) return true;
        return false;
    }
    else if(decision==2){
        if(rook(beginRow, beginCol, moveRow, moveCol, board, team, dead, error)) return true;
        return false;
    }
    else{*error = 5; return false;}//all errors is handeled above but for more check
}
void findking(wchar_t** board, int player, int* targetRow, int* targetCol){
    wchar_t target=(player==1)?L'♔':L'♚';
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(board[i][j]==target){
                *targetRow=i;
                *targetCol=j;
                return;
            }
        }
    }
}
bool isPlaceattacked(wchar_t** board, int *error, int player, wchar_t* Wteam, wchar_t* Bteam, int* Wdead, int* Bdead, wchar_t*** memory_board){ 
    int flag=0;
    int saveError = *error;
    wchar_t* oppteam=(player==1)?Wteam:Bteam;//to check if the team of opposite side 
    int* mydead=(player==1)?Bdead:Wdead; // white oppteam white my dead bdead myteam Bteam
    wchar_t* myteam=(player==1)?Bteam:Wteam;
    int destrow=0, destcol=0;
    findking(board, player, &destrow, &destcol);
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            int beginrow=i,begincol=j;
            int moveRow= destrow-i;
            int moveCol= destcol-j;
            for(int k=0; k<6; k++){
                if(board[i][j]==oppteam[k]){ // white oppteam white my dead bdead myteam Bteam
                    wchar_t target=board[i][j];
                    if(target == L'♜' || target == L'♖'){
                        if(rook(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error)){*error=0;return true;}
                    }
                    else if(target == L'♞' || target == L'♘'){
                        if(knight(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error)){*error=0;return true;}
                    }
                    else if(target == L'♝' || target == L'♗'){
                        if(bishop(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error)){*error=0;return true;}
                    }
                    else if(target == L'♚' || target == L'♔'){
                        if(king(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error, player)){*error=0;return true;}
                    }
                    else if(target == L'♛' || target == L'♕'){
                        
                        if(queen(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error)){*error=0;return true;}
                    }
                    else if(target == L'♟' || target == L'♙'){
                        if(pawn(beginrow, begincol, destrow, destcol, moveRow, moveCol, board, myteam, mydead, error, player)){*error=0;return true;}
                    }
                }
            }
        }
    }
    *error=saveError;//to reset error from theoritical moves
    return false;
}
bool canMove(wchar_t** board, wchar_t* myteam, wchar_t* oppteam, wchar_t* Wteam, wchar_t* Bteam, int* mydead, int* Wdead, int* Bdead, int *error, int player, wchar_t*** memory_board){
    int saveError = *error;
    save_move(memory_board, board, Wdead, Bdead);
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){//to get begining of piece
            int beginrow=i;
            int begincol=j;
            for(int k=0; k<6; k++){
                if(board[i][j]==oppteam[k]){//test if it is from opponent team
                    wchar_t target=board[i][j];
                    for(int x=0; x<8; x++){
                        for(int y=0; y<8; y++){//test all compinations of movement
                            int destrow=x;
                            int destcol=y;
                            int moveRow= destrow-beginrow;
                            int moveCol= destcol-begincol;

                            if(target == L'♜' || target == L'♖'){
                                if(rook(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error)){
                                    if(isPlaceattacked(board, error, !player, Wteam, Bteam, Wdead, Bdead, memory_board)) {
                                        save_move(memory_board, board, Wdead, Bdead);
                                        max_slot = saveSlot;
                                        max_slot -= 1;
                                        undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                        continue;
                                    }
                                    save_move(memory_board, board, Wdead, Bdead);
                                    max_slot = saveSlot;
                                    max_slot -= 2;
                                    undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                    *error=saveError;
                                    return true;
                                }
                            }
                            else if(target == L'♞' || target == L'♘'){
                                if(knight(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error)){
                                    if(isPlaceattacked(board, error, !player, Wteam, Bteam, Wdead, Bdead, memory_board)) {
                                        save_move(memory_board, board, Wdead, Bdead);
                                        max_slot = saveSlot;
                                        max_slot -= 1;
                                        undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                        continue;
                                    }
                                    save_move(memory_board, board, Wdead, Bdead);
                                    max_slot = saveSlot;
                                    max_slot -= 2;
                                    undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                    *error=saveError;
                                    return true;
                                }
                            }
                            else if(target == L'♝' || target == L'♗'){
                                if(bishop(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error)){
                                    if(isPlaceattacked(board, error, !player, Wteam, Bteam, Wdead, Bdead, memory_board)) {
                                        save_move(memory_board, board, Wdead, Bdead);
                                        max_slot = saveSlot;
                                        max_slot -= 1;
                                        undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                        continue;
                                    }
                                    save_move(memory_board, board, Wdead, Bdead);
                                    max_slot = saveSlot;
                                    max_slot -= 2;
                                    undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                    *error=saveError; 
                                    return true;
                                }
                            }
                            else if(target == L'♚' || target == L'♔'){
                                if(king(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error, player)){
                                    if(isPlaceattacked(board, error, !player, Wteam, Bteam, Wdead, Bdead, memory_board)) {
                                        save_move(memory_board, board, Wdead, Bdead);
                                        max_slot = saveSlot;
                                        max_slot -= 1;
                                        undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                        continue;
                                    }
                                    save_move(memory_board, board, Wdead, Bdead);
                                    max_slot = saveSlot;
                                    max_slot -= 2;
                                    undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                    *error=saveError;
                                    return true;
                                
                                }
                            }
                            else if(target == L'♛' || target == L'♕'){
                                if(queen(beginrow, begincol, moveRow, moveCol, board, myteam, mydead, error)){
                                    if(isPlaceattacked(board, error, !player, Wteam, Bteam, Wdead, Bdead, memory_board)) {
                                        save_move(memory_board, board, Wdead, Bdead);
                                        max_slot = saveSlot;
                                        max_slot -= 1;
                                        undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                        continue;
                                    }
                                    save_move(memory_board, board, Wdead, Bdead);
                                    max_slot = saveSlot;
                                    max_slot -= 2;
                                    undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                    *error=saveError;
                                    return true;
                                }
                            }
                            else if(target == L'♟' || target == L'♙'){
                                if(pawn(beginrow, begincol, destrow, destcol, moveRow, moveCol, board, myteam, mydead, error, player)){
                                    if(isPlaceattacked(board, error, !player, Wteam, Bteam, Wdead, Bdead, memory_board)) {
                                        save_move(memory_board, board, Wdead, Bdead);
                                        max_slot = saveSlot;
                                        max_slot -= 1;
                                        undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                        continue;
                                    }
                                    save_move(memory_board, board, Wdead, Bdead);
                                    max_slot = saveSlot;
                                    max_slot -= 2;
                                    undo_move(memory_board, board, error, Wdead, Bdead, 1);
                                    *error=saveError;
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    *error=saveError;
    return false;
}
void dealloction(wchar_t** twoDim, wchar_t*** threeDim){//the memory and board and they are fixed size no need to dermine dimension
    for(int i=0; i<500; i++){
        for(int r=0; r<11; r++){
            free(threeDim[i][r]);
        }
        free(threeDim[i]);
    }
    free(threeDim);

    for(int i=0; i<8; i++){
        free(twoDim[i]);
    }
    free(twoDim);
}
void movement(int srcRow, int srcCol, int destRow, int destCol, wchar_t** board, int player, int *error, wchar_t* Wteam, wchar_t* Bteam, int* Wdead, int* Bdead, wchar_t*** memory_board){
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
        case L'♚':// White King (inverted)
            king(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error, player);
            //error no need to * or & as it was from caller int* error
            break;
        case L'♔':// Black King
            king(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error, player);
            break;
        case L'♛':// White Queen
            queen(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♕':// Black Queen
            queen(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♜':// White Rook
            rook(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♖':// Black Rook
            rook(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♝':// White Bishop
            bishop(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♗':// Black Bishop
            bishop(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♞':// White Knight
            knight(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♘':// Black Knight
            knight(srcRow, srcCol, moveRow, moveCol, board, oppteam, oppdead, error);
            break;
        case L'♟':// White Pawn
            pawn(srcRow, srcCol, destRow, destCol, moveRow, moveCol, board, oppteam, oppdead, error, player);
            if(destRow==7)pormotion(player,error,board,destCol);
            break;
        case L'♙':// Black Pawn
            pawn(srcRow, srcCol, destRow, destCol, moveRow, moveCol, board, oppteam, oppdead, error, player);
            if(destRow==0)pormotion(player,error,board,destCol);
            break;
    }
    wchar_t* myteam=(player==1)?Wteam:Bteam;//to use on winning conditions
    int* mydead=(player==1)?Wdead:Bdead;
    if(isPlaceattacked(board, error, !player, Wteam, Bteam, Wdead, Bdead, memory_board)){*error=6;return;}//to check if done invalid move by move piece and his king in check

    if(!(canMove(board, myteam, oppteam, Wteam, Bteam, mydead, Wdead, Bdead, error, !player, memory_board))) {

        if(isPlaceattacked(board, error, player, Wteam, Bteam, Wdead, Bdead, memory_board)) {
            wprintf(L"Checkmate!\n");
            wprintf(L"You win!\n");
            dealloction(board,memory_board);
            exit(0);//to end game
        }
        else {
            wprintf(L"Stalemate!\n");
            wprintf(L"It's Draw!\n");
            dealloction(board,memory_board);
            exit(0);
        }

    }
    else {
        save_move(memory_board, board, Wdead, Bdead);
        if(isPlaceattacked(board, error, player, Wteam, Bteam, Wdead, Bdead, memory_board)) {
            *error = 9;
            return;
        }
    }

}