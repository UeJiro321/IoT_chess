#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

#include <iostream>
#include <cstdint>
#include <memory>
using namespace std;


class Position {
    public:
        Position();
        Position(int x, int y);
        int getRow();
        int getCol();
    private:
        int row;
        int col;
};

class Piece {
    public:
        Piece();
        Piece(char symbol, bool color);
        bool getColor() const;
        char getSymbol() const;
        virtual bool canMove(Position from, Position to) = 0;
    private:
        bool color;
        char symbol;
};


class MoveCommand {
    public:
        MoveCommand();
        // state: 0 - selecting piece, 1 - moving to
        int state;
        // turn: true - white's turn, false - black
        bool turn;
        // isEnter: true - enter, false - not enter
        bool isEnter;
        // direction: 0 - up, 1 - down, 2 - left, 3 - right
        int dir;
        // isTimeout: true - timeout, false - not timeout
        bool isTimeout;
};


// Entity Layer
class Board {
    public:
        Board();
        bool movePiece(Position from, Position to);
        const Piece* getPiece(Position pos) const;
        bool moveCursor(int dir);
        Position getCursorPos();
    private:
        std::unique_ptr<Piece> square[8][8];
        Position cursorPos;
};

// User Interface Layer
class HardwareInput {
    public:
        MoveCommand getMove();
    private:
        int fd;
        MoveCommand decodeCommand(uint32_t data);
};

class ConsoleInput {
    public:
        MoveCommand getMove();
};

class ConsoleOutput {
    public:
        void drawBoard(const Board& board);
        void drawResult(bool winner);
};

// Control Layer
class Game {
    public:
        Game();
        void play();
        void switchTurn();
        bool checkWin();
        void handleCommand(MoveCommand cmd);
    private:
        int gameState;
        Board board;
        bool currentTurn;
        HardwareInput input;
        ConsoleOutput output;
        Position from;
};

class Pawn : public Piece {
    public:
        Pawn(bool color);
        bool canMove(Position from, Position to) override;
};

class Rook : public Piece {
    public:
        Rook(bool color);
        bool canMove(Position from, Position to) override;
};

class Knight : public Piece {
    public:
        Knight(bool color);
        bool canMove(Position from, Position to) override;
};

class Bishop : public Piece {
    public:
        Bishop(bool color);
        bool canMove(Position from, Position to) override;
};

class Queen : public Piece {
    public:
        Queen(bool color);
        bool canMove(Position from, Position to) override;
};

class King : public Piece {
    public:
        King(bool color);
        bool canMove(Position from, Position to) override;
};

class Empty : public Piece {
    public:
        Empty();
        bool canMove(Position from, Position to) override;
};

#endif // _COMPONENTS_H_