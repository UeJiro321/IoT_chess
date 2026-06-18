#include <iostream>
#include <cstdint>
using namespace std;

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
        void drawBoard(Board board);
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

class MoveCommand {
    private:
        int state;
        bool isEnter;
        int dir;
        bool isTimeout;
};

// Entity Layer
class Board {
    public:
        Board();
        bool movePiece(Position from, Position to);
        Piece *getPiece(Position pos);
        bool moveCursor(int dir);
        Position getCursorPos();
    private:
        Piece square[8][8];
        Position cursorPos;
};

class Position {
    public:
        Position(int x, int y);
        bool canMoveTo(Position pos);
        int getX();
        int getY();
    private:
        int row;
        int col;
};

class Piece {
    public:
        Piece(char symbol, bool color);
        bool getColor();
        char getSymbol();
    private:
        bool color;
        char symbol;
};

class Pawn : public Piece {
    public:
        Pawn(bool color);
};

class Rook : public Piece {
    public:
        Rook(bool color);
};

class Knight : public Piece {
    public:
        Knight(bool color);
};

class Bishop : public Piece {
    public:
        Bishop(bool color);
};

class Queen : public Piece {
    public:
        Queen(bool color);
};

class King : public Piece {
    public:
        King(bool color);
};

class Empty : public Piece {
    public:
        Empty();
};