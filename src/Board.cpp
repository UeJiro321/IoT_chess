#include <algorithm>
#include "Board.h"
#include "Piece.h"
#include "stdio.h"

Board::Board(){
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
    if(square[from.getRow()][from.getCol()] == nullptr) {
        return false;
    }

    Piece* movingPiece = square[from.getRow()][from.getCol()].get();
    Piece* targetPiece = square[to.getRow()][to.getCol()].get();

    // 同じ色の駒が目的地にある場合は移動不可
    if(targetPiece != nullptr &&
       targetPiece->getColor() == movingPiece->getColor()) {
        return false;
    }

    if(movingPiece->onMove(from, to)){
        bool isCastling = canCastle(from, to);

        if (isCastling) {
            int row = from.getRow();
        
            // kingside
            if (to.getCol() > from.getCol()) {
                square[row][6] = std::move(square[row][4]);
                square[row][5] = std::move(square[row][7]);
            
                square[row][6]->setMoved();
                square[row][5]->setMoved();
            }
            // queenside
            else {
                square[row][2] = std::move(square[row][4]);
                square[row][3] = std::move(square[row][0]);
            
                square[row][2]->setMoved();
                square[row][3]->setMoved();
            }
            return true;
        }
        if(movingPiece->canMove(from, to, *this)){
            // 駒を取る場合、moveする前に記録する
            if(targetPiece != nullptr){
                char capturedSymbol = targetPiece->getSymbol();

                if(targetPiece->getColor()){
                    capturedRedPieces.push_back(capturedSymbol);
                } else {
                    capturedBluePieces.push_back(capturedSymbol);
                }
            }

            square[to.getRow()][to.getCol()] =
                std::move(square[from.getRow()][from.getCol()]);

            square[from.getRow()][from.getCol()] = nullptr;

            promotePawn(to);

            return true;
        }
    }

    return false;
}

void Board::promotePawn(Position pos)
{
    Piece* piece = square[pos.getRow()][pos.getCol()].get();

    if (piece == nullptr) {
        return;
    }

    char symbol = piece->getSymbol();

    if (symbol != 'P' && symbol != 'p') {
        return;
    }

    bool color = piece->getColor();

    // true 側のポーンは row 7 で昇格
    if (color && pos.getRow() == 7) {
        square[pos.getRow()][pos.getCol()] = std::make_unique<Queen>(true);
    }
    // false 側のポーンは row 0 で昇格
    else if (!color && pos.getRow() == 0) {
        square[pos.getRow()][pos.getCol()] = std::make_unique<Queen>(false);
    }
}

bool Board::canCastle(Position from, Position to) const
{
    const Piece* king = getPiece(from);

    if (king == nullptr) {
        return false;
    }

    if (king->getSymbol() != 'K' && king->getSymbol() != 'k') {
        return false;
    }

    if (king->hasMoved()) {
        return false;
    }

    // King は同じ行で横に2マス動く
    if (from.getRow() != to.getRow()) {
        return false;
    }

    int colDiff = to.getCol() - from.getCol();

    if (abs(colDiff) != 2) {
        return false;
    }

    int row = from.getRow();

    // kingside: 右側キャスリング
    int rookCol;
    if (colDiff == 2) {
        rookCol = 7;
    }
    // queenside: 左側キャスリング
    else {
        rookCol = 0;
    }

    const Piece* rook = getPiece(Position(row, rookCol));

    if (rook == nullptr) {
        return false;
    }

    if (rook->getSymbol() != 'R' && rook->getSymbol() != 'r') {
        return false;
    }

    if (rook->getColor() != king->getColor()) {
        return false;
    }

    if (rook->hasMoved()) {
        return false;
    }

    // King と Rook の間に駒がないか確認
    int start = std::min(from.getCol(), rookCol);
    int end = std::max(from.getCol(), rookCol);

    for (int col = start + 1; col < end; col++) {
        if (getPiece(Position(row, col)) != nullptr) {
            return false;
        }
    }

    return true;
}

bool Board::isKingAlive(bool color) const
{
    char kingSymbol = color ? 'K' : 'k';

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece* p = square[row][col].get();

            if (p != nullptr &&
                p->getColor() == color &&
                p->getSymbol() == kingSymbol) {
                return true;
            }
        }
    }

    return false;
}

const std::vector<char>& Board::getCapturedRedPieces() const
{
    return capturedRedPieces;
}

const std::vector<char>& Board::getCapturedBluePieces() const
{
    return capturedBluePieces;
}