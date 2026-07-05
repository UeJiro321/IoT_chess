#ifndef _GAME_H_
#define _GAME_H_

#include "Core.h"
#include "Board.h"
#include "UI.h"

#ifdef USE_HARDWARE_INPUT
#include "HardwareInput.h"
#else
#include "ConsoleInput.h"
#endif

#include <string>

class Game {
public:
    Game();
    void play();
    bool checkWin() const;
    bool handleCommand(MoveCommand cmd);

private:
    void updateGameResult();

private:
    int gameState;
    MoveCommand cmd;
    Position cursorPos;
    Board board;

#ifdef USE_HARDWARE_INPUT
    HardwareInput input;
#else
    ConsoleInput input;
#endif

    ConsoleOutput output;
    Position from;

    bool hasSelectedPiece;
    std::string lastMessage;

    bool gameOver;
    bool winnerTurn;
};

#endif // _GAME_H_