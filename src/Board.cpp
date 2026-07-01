#include "Board.h"
#include "Piece.h"
#include "stdio.h"

Board::Board(){
    // Initialize pieces
    for(int i = 0; i < 8; i++){
        square[1][i] = std::make_unique<Pawn>(true);
        square[6][i] = std::make_unique<Pawn>(false);
    }
    square[0][0] = std::make_unique<Rook>(true);
    square[0][7] = std::make_unique<Rook>(true);
    square[7][0] = std::make_unique<Rook>(false);
    square[7][7] = std::make_unique<Rook>(false);
    square[0][1] = std::make_unique<Knight>(true);
    square[0][6] = std::make_unique<Knight>(true);
    square[7][1] = std::make_unique<Knight>(false);
    square[7][6] = std::make_unique<Knight>(false);
    square[0][2] = std::make_unique<Bishop>(true);
    square[0][5] = std::make_unique<Bishop>(true);
    square[7][2] = std::make_unique<Bishop>(false);
    square[7][5] = std::make_unique<Bishop>(false);
    square[0][3] = std::make_unique<Queen>(true);
    square[0][4] = std::make_unique<King>(true);
    square[7][3] = std::make_unique<Queen>(false);
    square[7][4] = std::make_unique<King>(false);
}

const Piece* Board::getPiece(Position pos) const{
    return square[pos.getRow()][pos.getCol()].get();
}

bool Board::movePiece(Position from, Position to){
    // if(square[from.getRow()][from.getCol()]->getColor() == square[to.getRow()][to.getCol()]->getColor()){
    //     return false;
    // }
    if(square[from.getRow()][from.getCol()] == nullptr) return false;
    if(square[from.getRow()][from.getCol()]->onMove(from, to)){
        if(square[from.getRow()][from.getCol()]->canMove(from, to, *this)){
            printf("Valid move.\n");
            square[to.getRow()][to.getCol()] = std::move(square[from.getRow()][from.getCol()]);
            square[from.getRow()][from.getCol()] = nullptr;
            return true;
        }
    }
    return false;
}