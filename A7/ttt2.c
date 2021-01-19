#include "ttt.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

void init_boards() {

    for(int i = 0; i < HSIZE; i++) //loops through the whole hash table
    {
        htable[i].init = 0; //sets every board node value to init 0 to indicate that it is empty
    }

}

int depth( Board board ) {
    int counter = 0; //counter that will store the depth of the board

    for(int i = 0; i < 9; i++) //loops 9 times (9 total spaces on the board)
    {
        if(board[pos2idx[i]] == 'X' || board[pos2idx[i]] == 'O') //if the spot is filled
        {
            counter++; //increment the counter
        }
    }

    return counter; //returns the depth, max value of 9
}

char winner( Board board ) {

    int counterX = 0; //counts how many X's in a winnable row
    int counterO = 0; //count's how many O's in a winnable row
    int winX = 0; //sets to one if X wins
    int winO = 0; //sets to one if O wins

    for(int i = 0; i < 8; i++) //loops through all the possible win conditions
    {
        for(int j = 0; j < 3; j++) //goes through the three spaces that result in the win condition
        {
            if (board[pos2idx[WINS[i][j]]] == 'X') //if the value in a slot of the current win condtion is an X
            {
                counterX++; //increment the X counter
            }
            else if (board[pos2idx[WINS[i][j]]] == 'O') //if the value in a slot of the current win condtion is an O
            {
                counterO++; //increment the O counter
            }
            
        }
        if (counterO == 3) //if there are 3 O's in a line
        {
            winO = 1; //O wins
            i = 8; //exit loop
        }
        else if (counterX == 3) //if there are 3 X's in a line
        {
            winX = 1; //X wins
            i = 8; //exit loop
        }
        else { //if no one won with the given condition
            counterO = 0; //reset both counter values
            counterX = 0;
        }
    }

    if(winO) //if O wins
    {
        return('O');
    }
    else if(winX){ //if X wins
        return('X');
    }
    else if(depth(board) == 9) //if there is a tie (all spots are filled with no winner)
    {
        return('-');
    }
    else{ //if the game is still going
        return('?');
    }

}

char turn( Board board ) {
    if(depth(board) == 9 || winner(board) != '?') //if the board is full or a winner has been decided
    {
        return('-');
    }
    else if(depth(board) % 2 == 0) //if it is X's turn
    {
        return('X');
    }
    else { //if it is O's turn
        return('O');
    }
}

void init_board( Board board ) {

   int index = board_hash(board); //gets the hash value of the passed in board
   htable[index].init = 1; //sets init to one to indicate it has been initialized
   htable[index].turn = turn(board); //sets the turn
   htable[index].depth = depth(board); //sets the depth
   strcpy(htable[index].board, board); //copies the passed in board into the hash table
   htable[index].winner = winner(board); //sets the winner

}

void join_graph (Board board) {

    int index = board_hash(board); //gets the hash value of the passed in board
    for(int i = 0; i < 9; i++) //loops through every position
    {
        if(htable[index].board[pos2idx[i]] == 'X' || htable[index].board[pos2idx[i]] == 'O') //if there is a X or O at the current spot
        {
            htable[index].move[i] = -1; //that move cannot be made
        }
        else{ //if the move can be made
            Board newBoard; //new board that will be linked
            strcpy(newBoard, htable[index].board); //copy the old board into the new borad
            newBoard[pos2idx[i]] = turn(newBoard); //make the corresponding move on the new board
            int newHash = board_hash(newBoard); //calculate the hash value of the new board
            htable[index].move[i] = newHash; //set the move of the old board to the newly computed hash of the old board
            if(htable[newHash].init == 0 && winner(board) == '?') //if the board at the new hash doesnt exist yet and a winner has not been decided
            {
                init_board(newBoard); //initialize the new board
                join_graph(newBoard); //recursively call join graph to link the board
            } 
        }
    }
}

void compute_score() {
    int score = 0;
    for(int j = 9; j >= 0; j--) //loops from deepest depth (9) to lowest
    {
        for(int i = 0; i < HSIZE; i++) //loops through the whole hash table
        {
            if(htable[i].depth == j) //if the nodes depth is the current j index (9, 8, 7, 6...)
            {
                if(htable[i].winner == 'X') { //if there is a winner of X
                    htable[i].score = 1;
                }
                else if(htable[i].winner == 'O') { //if there is a winner of O
                    htable[i].score = -1;
                }
                else if(htable[i].winner == '-') { //if there is a tie
                    htable[i].score = 0;
                }
                else { //if there is no winner
                    
                        if(htable[i].turn == 'O') //if it is O's turn
                        {
                            score = 1; //set score to the largest possible value
                            for(int k = 0; k < 9; k++) //loop through all the possible moves
                            {
                                if(htable[i].move[k] != -1) //if the move is valid
                                {
                                    if(htable[htable[i].move[k]].score < score) //if the move's score is less than the current score
                                    {
                                        score = htable[htable[i].move[k]].score; //that is the new score
                                    }
                                }
                            }
                        }
                        if(htable[i].turn == 'X') //if it is X's turn
                        {
                            score = -1; //set score to the smallest possible value
                            for(int k = 0; k < 9; k++) //loop through all the possible moves
                            {
                                if(htable[i].move[k] != -1) //if the move is valid
                                {
                                    if(htable[htable[i].move[k]].score > score) //if the move's score is more than the current score
                                    {
                                        score = htable[htable[i].move[k]].score; //that is the new score
                                    }
                                }
                            }
                        }
                    
                    htable[i].score = score; //sets the new score of the node
                }

            }
        }
            
    }
}

int best_move( int board ) {

    int move = 0; //will store best move
    int winner = 0; //will store if there is a winning option

    if(htable[board].turn == 'X') //if it's X's turn
    {
        int bestmove = -1; //set the current best move score to -1 (the lowest possible score)
        
        for(int i = 0; i < 9; i++) //loops through all the moves
        {
            if(htable[htable[board].move[i]].score >= bestmove && htable[board].move[i] != -1 && winner != 1) //if there is a better X score available (or same score) and there has not been a winning move
            {
                bestmove = htable[htable[board].move[i]].score; //the new best move score is set to the current move's score
                move = i; //the index of the move is stored
                if(htable[htable[board].move[i]].winner == 'X') //if the move is a winning move
                {    
                    winner = 1; //set the winner flag (prevents other moves overwritting winning move)
                }
            }
        }
    }
    else if(htable[board].turn == 'O') //if it's O's turn
    {
        int bestmove = 1; //set the current best move score to 1 (the highest possible score)
        for(int i = 0; i < 9; i++) //loop through all the moves
        {
            if(htable[htable[board].move[i]].score <= bestmove && htable[board].move[i] != -1 && winner != 1) //if there is a better O score available (or same score) and there has not been a winning move
            {
                bestmove = htable[htable[board].move[i]].score; //the new best move score is set to the current move's score
                move = i; //the index of the move is stored
                if(htable[htable[board].move[i]].winner == 'O') //if the move is a winning move
                {
                    winner = 1; //set the winner flag (prevents other moves overwritting winning move)
                }
            }
        }
    }
    return move; //returns the best possible move
}