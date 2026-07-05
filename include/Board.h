#ifndef _BOARD_H_
#define _BOARD_H_

#include "Core.h"
#include "Piece.h"

#include <memory>
#include <vector>

class Board {
public:
    Board();

    const Piece* getPiece(Position pos) const;
    bool movePiece(Position from, Position to);

    const std::vector<char>& getCapturedRedPieces() const;
    const std::vector<char>& getCapturedBluePieces() const;

    void promotePawn(Position pos);
    bool canCastle(Position from, Position to) const;
    bool isKingAlive(bool color) const;

private:
    std::unique_ptr<Piece> square[8][8];

    std::vector<char> capturedRedPieces;
    std::vector<char> capturedBluePieces;
};

#endif // _BOARD_H_