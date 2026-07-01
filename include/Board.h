#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
#include <memory>
#include "Core.h" 
#include "Piece.h" 

class Board {
public:
    Board();
    // 抽象クラスはポインタで保持する
    // std::vector<std::unique_ptr<Piece>> white_loss;
    // std::vector<std::unique_ptr<Piece>> black_loss;

    bool movePiece(Position from, Position to);
    const Piece* getPiece(Position pos) const;
    bool moveCursor(int dir);
    // Position getCursorPos() const;
private:
    std::unique_ptr<Piece> square[8][8];
};

#endif // _BOARD_H_