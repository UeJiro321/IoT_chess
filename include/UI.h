#ifndef _UI_H_
#define _UI_H_

#include "Core.h"
#include "Board.h"
#include <iostream>
#include <cstdint>

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
    void drawBoard(const Board& board, Position cursorPos);
    void drawResult(bool winner);
};


#endif // _UI_H_