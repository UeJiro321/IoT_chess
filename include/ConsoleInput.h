#ifndef _CONSOLE_INPUT_H_
#define _CONSOLE_INPUT_H_

#include "Core.h"

#include <cstdint>
#include <queue>
#include <string>

class ConsoleInput {
public:
    ConsoleInput();

    MoveCommand getMove();

    void gameStart();
    void notifyDone(bool success);
    void clearDone();

private:
    MoveCommand decodeCommand(uint32_t data);

    bool parseDebugMove(
        const std::string& line,
        MoveCommand& selectCmd,
        MoveCommand& moveCmd
    );

    bool parseColor(const std::string& text, bool& turn) const;
    bool parseSquare(const std::string& text, int& row, int& col) const;

private:
    bool softwareDone;

    std::queue<MoveCommand> pendingCommands;
};

#endif // _CONSOLE_INPUT_H_