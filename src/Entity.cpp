#include "components.h"

// Entity Layer
Board::Board(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            square[i][j] = std::make_unique<Empty>();
        }
    }
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
    cursorPos = Position(0, 0);
}

bool Board::movePiece(Position from, Position to){
    // ポインタ経由で canMove を呼び出す
    if(square[from.getRow()][from.getCol()]->canMove(from, to)){
        // ポインタの所有権を移動（std::moveを使用）
        square[to.getRow()][to.getCol()] = std::move(square[from.getRow()][from.getCol()]);
        square[from.getRow()][from.getCol()] = std::make_unique<Empty>();
        return true;
    }
    return false;
}

const Piece* Board::getPiece(Position pos) const{
    return square[pos.getRow()][pos.getCol()].get();
}

bool Board::moveCursor(int dir){
    int row = cursorPos.getRow();
    int col = cursorPos.getCol();
    switch(dir){
        case 0: // up
            if(row > 0) cursorPos = Position(row - 1, col);
            break;
        case 1: // down
            if(row < 7) cursorPos = Position(row + 1, col);
            break;
        case 2: // left
            if(col > 0) cursorPos = Position(row, col - 1);
            break;
        case 3: // right
            if(col < 7) cursorPos = Position(row, col + 1);
            break;
        default:
            return false;
    }
    return true;
}

Position Board::getCursorPos(){
    return cursorPos;
}

Position::Position(int x, int y){
    this->row = x;
    this->col = y;
}
Position::Position(){
    this->row = 0;
    this->col = 0;
}
int Position::getRow(){
    return this->row;
}
int Position::getCol(){
    return this->col;
}

Piece::Piece(){
    this->color = false;
    this->symbol = ' ';
}
Piece::Piece(char symbol, bool color){
    this->color = color;
    this->symbol = symbol;
}
bool Piece::getColor() const {
    return this->color;
}
char Piece::getSymbol() const {
    return this->symbol;
}

// bool Piece::canMove(Position from, Position to){
//     return true;
// }

Pawn::Pawn(bool color) : Piece(color ? 'P' : 'p', color) {}
bool Pawn::canMove(Position from, Position to){
    int dir = getColor() ? 1 : -1;
    if(from.getCol() == to.getCol()){
        if(to.getRow() - from.getRow() == dir){
            return true;
        }
        if((getColor() && from.getRow() == 1) || (!getColor() && from.getRow() == 6)){
            if(to.getRow() - from.getRow() == 2 * dir){
                return true;
            }
        }
    } else if(abs(from.getCol() - to.getCol()) == 1 && to.getRow() - from.getRow() == dir){
        // Capture
        return true;
    }
    return false;
}

Rook::Rook(bool color) : Piece(color ? 'R' : 'r', color) {}
bool Rook::canMove(Position from, Position to){
    if(from.getRow() == to.getRow() || from.getCol() == to.getCol()){
        return true;
    }
    return false;
}
Knight::Knight(bool color) : Piece(color ? 'N' : 'n', color){}
bool Knight::canMove(Position from, Position to){
    int rowDiff = abs(from.getRow() - to.getRow());
    int colDiff = abs(from.getCol() - to.getCol());
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}
Bishop::Bishop(bool color) : Piece(color ? 'B' : 'b', color) {}
bool Bishop::canMove(Position from, Position to){
    int rowDiff = abs(from.getRow() - to.getRow());
    int colDiff = abs(from.getCol() - to.getCol());
    return rowDiff == colDiff;
}
Queen::Queen(bool color) : Piece(color ? 'Q' : 'q', color) {}
bool Queen::canMove(Position from, Position to){
    int rowDiff = abs(from.getRow() - to.getRow());
    int colDiff = abs(from.getCol() - to.getCol());
    return from.getRow() == to.getRow() || from.getCol() == to.getCol() || rowDiff == colDiff;
}
King::King(bool color) : Piece(color ? 'K' : 'k', color) {}
bool King::canMove(Position from, Position to){
    int rowDiff = abs(from.getRow() - to.getRow());
    int colDiff = abs(from.getCol() - to.getCol());
    return rowDiff <= 1 && colDiff <= 1;
}
Empty::Empty() : Piece(' ', false) {}
bool Empty::canMove(Position from, Position to){
    return false;
}