#include "Piece.h"
#include "Board.h"
#include <algorithm>

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

// Pawn, Rook, Knight, Bishop, Queen, King, and Empty class implementations
Pawn::Pawn(bool color) : Piece(color ? 'P' : 'p', color) {}
bool Pawn::onMove(Position from, Position to){
    int dir = getColor() ? 1 : -1;
    // 前方への移動
    if(from.getCol() == to.getCol()){
        // 1マス前方への移動
        if(to.getRow() - from.getRow() == dir){
            return true;
        }

        // 2マス前方への移動（初期位置のみ）
        if((getColor() && from.getRow() == 1) || (!getColor() && from.getRow() == 6)){
            if(to.getRow() - from.getRow() == 2 * dir){
                return true;
            }
        }
    } else if(abs(from.getCol() - to.getCol()) == 1 && to.getRow() - from.getRow() == dir){
        // 斜め方向のキャプチャ
        return true;
    }
    return false;
}
bool Pawn::canMove(Position from, Position to, const Board& board) const{
    int dir = getColor() ? 1 : -1;
    if(abs(from.getCol() - to.getCol()) == 1 && to.getRow() - from.getRow() == dir){
        if(board.getPiece(to)->getColor() != getColor()){
            return true;
        }
    }
    // 前進先に駒があれば進めない
    if(board.getPiece(to) == nullptr){
        return true;
    }
    return false;
}


Rook::Rook(bool color) : Piece(color ? 'R' : 'r', color) {}
bool Rook::onMove(Position from, Position to){
    if(from.getRow() == to.getRow() || from.getCol() == to.getCol()){
        return true;
    }
    return false;
}

bool Rook::canMove(Position from, Position to, const Board& board) const {
    // 縦移動
    if(from.getCol() == to.getCol()){
        int from_pos = from.getRow();
        int to_pos = to.getRow();
        
        // std::min, std::max を使うと from > to の場合の分岐(if/else)を省略できます
        int start = std::min(from_pos, to_pos);
        int end = std::max(from_pos, to_pos);

        // 間のマスに障害物がないかチェック (修正済みの for 文)
        for(int i = start + 1; i < end; i++){
            if(board.getPiece(Position(i, from.getCol())) != nullptr) return false;
        }
    }
    // 横移動
    else if(from.getRow() == to.getRow()){
        int from_pos = from.getCol();
        int to_pos = to.getCol();
        
        int start = std::min(from_pos, to_pos);
        int end = std::max(from_pos, to_pos);

        // 間のマスに障害物がないかチェック (修正済みの for 文)
        for(int i = start + 1; i < end; i++){
            if(board.getPiece(Position(from.getRow(), i)) != nullptr) return false;
        }
    }
    // 縦でも横でもない移動は不可
    else {
        return false;
    }

    // --- ここまで到達していれば、間のマスには何もない ---

    // 最終的な目的地(to)の判定（縦横共通なので外に出す）
    const Piece* targetPiece = board.getPiece(to);
    
    // 目的地が空なら移動可能
    if(targetPiece == nullptr) {
        return true;
    }
    // 目的地に敵の駒があれば移動可能（取る）
    else if(targetPiece->getColor() != getColor()) {
        return true;
    }

    // 目的地に味方の駒がある場合は移動不可
    return false;
}

Knight::Knight(bool color) : Piece(color ? 'N' : 'n', color){}
bool Knight::onMove(Position from, Position to){
    int rowDiff = abs(from.getRow() - to.getRow());
    int colDiff = abs(from.getCol() - to.getCol());
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}
bool Knight::canMove(Position from, Position to, const Board& board) const{
    if(board.getPiece(to) == nullptr || board.getPiece(to)->getColor() != getColor()) return true;
    else return false;
}

Bishop::Bishop(bool color) : Piece(color ? 'B' : 'b', color) {}
bool Bishop::onMove(Position from, Position to){
    int rowDiff = abs(from.getRow() - to.getRow());
    int colDiff = abs(from.getCol() - to.getCol());
    return rowDiff == colDiff;
}
bool Bishop::canMove(Position from, Position to, const Board& board) const {
    int rowDiff = to.getRow() - from.getRow();
    int colDiff = to.getCol() - from.getCol();

    // 斜め移動でない場合は false
    if (abs(rowDiff) != abs(colDiff) || rowDiff == 0) {
        return false;
    }

    // 進行方向 (+1 または -1) を決定
    int rowDir = (rowDiff > 0) ? 1 : -1;
    int colDir = (colDiff > 0) ? 1 : -1;
    
    // 移動するマスの数
    int distance = abs(rowDiff);

    // 間のマスに障害物がないかチェック
    for (int i = 1; i < distance; ++i) {
        int checkRow = from.getRow() + (i * rowDir);
        int checkCol = from.getCol() + (i * colDir);
        if (board.getPiece(Position(checkRow, checkCol)) != nullptr) {
            return false; // 障害物あり
        }
    }

    // 最終目的地(to)の判定
    const Piece* targetPiece = board.getPiece(to);
    if (targetPiece == nullptr || targetPiece->getColor() != getColor()) {
        return true;
    }
    return false; // 目的地に味方の駒がある
}

Queen::Queen(bool color) : Piece(color ? 'Q' : 'q', color) {}
bool Queen::onMove(Position from, Position to){
    int rowDiff = abs(from.getRow() - to.getRow());
    int colDiff = abs(from.getCol() - to.getCol());
    return from.getRow() == to.getRow() || from.getCol() == to.getCol() || rowDiff == colDiff;
}
bool Queen::canMove(Position from, Position to, const Board& board) const {
    int rowDiff = to.getRow() - from.getRow();
    int colDiff = to.getCol() - from.getCol();

    // 縦・横の移動 (Rookと同じロジック)
    if (rowDiff == 0 || colDiff == 0) {
        // 進行方向 (+1, -1, または 0)
        int rowDir = (rowDiff > 0) ? 1 : ((rowDiff < 0) ? -1 : 0);
        int colDir = (colDiff > 0) ? 1 : ((colDiff < 0) ? -1 : 0);
        int distance = std::max(abs(rowDiff), abs(colDiff));

        for (int i = 1; i < distance; ++i) {
            int checkRow = from.getRow() + (i * rowDir);
            int checkCol = from.getCol() + (i * colDir);
            if (board.getPiece(Position(checkRow, checkCol)) != nullptr) {
                return false;
            }
        }
    } 
    // 斜めの移動 (Bishopと同じロジック)
    else if (abs(rowDiff) == abs(colDiff)) {
        int rowDir = (rowDiff > 0) ? 1 : -1;
        int colDir = (colDiff > 0) ? 1 : -1;
        int distance = abs(rowDiff);

        for (int i = 1; i < distance; ++i) {
            int checkRow = from.getRow() + (i * rowDir);
            int checkCol = from.getCol() + (i * colDir);
            if (board.getPiece(Position(checkRow, checkCol)) != nullptr) {
                return false;
            }
        }
    } 
    // 縦・横・斜め以外の移動は不可
    else {
        return false;
    }

    // 最終目的地(to)の判定
    const Piece* targetPiece = board.getPiece(to);
    if (targetPiece == nullptr || targetPiece->getColor() != getColor()) {
        return true;
    }
    return false;
}

King::King(bool color) : Piece(color ? 'K' : 'k', color) {}
bool King::onMove(Position from, Position to){
    int rowDiff = abs(from.getRow() - to.getRow());
    int colDiff = abs(from.getCol() - to.getCol());
    return rowDiff <= 1 && colDiff <= 1;
}
bool King::canMove(Position from, Position to, const Board& board) const {
    int rowDiff = abs(to.getRow() - from.getRow());
    int colDiff = abs(to.getCol() - from.getCol());

    // 1マスしか動けないため、間の障害物チェックは不要
    // 移動範囲が1マス以内かどうかの判定 (全く動かないのはNG)
    if (rowDiff > 1 || colDiff > 1 || (rowDiff == 0 && colDiff == 0)) {
        return false;
    }

    // 最終目的地(to)の判定
    const Piece* targetPiece = board.getPiece(to);
    if (targetPiece == nullptr || targetPiece->getColor() != getColor()) {
        return true; // 空きマス、または敵の駒
    }
    
    return false; // 味方の駒
}