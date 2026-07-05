#ifndef _PIECE_H_
#define _PIECE_H_

#include <Core.h>

class Board;

class Piece {
public:
    Piece();
    Piece(char symbol, bool color);
    virtual ~Piece() = default;

    bool getColor() const;
    char getSymbol() const;

    virtual bool onMove(Position from, Position to) = 0;
    virtual bool canMove(Position from, Position to, const Board& board) const = 0;
    bool hasMoved() const { return moved; }
    void setMoved() { this->moved = true; }
private:
    bool color; // true for white, false for black
    char symbol;
    bool moved;
};

class Pawn : public Piece {
    public:
        Pawn(bool color);
        bool onMove(Position from, Position to) override;
        bool canMove(Position from, Position to, const Board& board) const override;
};

class Rook : public Piece {
    public:
        Rook(bool color);
        bool onMove(Position from, Position to) override;
        bool canMove(Position from, Position to, const Board& board) const override;
};

class Knight : public Piece {
    public:
        Knight(bool color);
        bool onMove(Position from, Position to) override;
        bool canMove(Position from, Position to, const Board& board) const override;
};

class Bishop : public Piece {
    public:
        Bishop(bool color);
        bool onMove(Position from, Position to) override;
        bool canMove(Position from, Position to, const Board& board) const override;
};

class Queen : public Piece {
    public:
        Queen(bool color);
        bool onMove(Position from, Position to) override;
        bool canMove(Position from, Position to, const Board& board) const override;
};

class King : public Piece {
    public:
        King(bool color);
        bool onMove(Position from, Position to) override;
        bool canMove(Position from, Position to, const Board& board) const override;
};

class Empty : public Piece {
    public:
        Empty();
        bool onMove(Position from, Position to) override;
        bool canMove(Position from, Position to, const Board& board) const override;
};

#endif // _PIECE_H_