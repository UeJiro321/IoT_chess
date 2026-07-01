#ifndef _GAME_H_
#define _GAME_H_

#include "Core.h"
#include "Board.h"
#include "UI.h"

class Game {
public:
    Game();
    void play();
    bool checkWin() const; // playing/end 
    void handleCommand(MoveCommand cmd);
private:
    int gameState;
    MoveCommand cmd;
    Position cursorPos;
    Board board;
    ConsoleInput input;
    // HardwareInput input;
    ConsoleOutput output;
    Position from;
};

#endif // _GAME_H_