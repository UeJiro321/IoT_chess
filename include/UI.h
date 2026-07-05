#ifndef _UI_H_
#define _UI_H_

#include "Board.h"
#include "Core.h"

#include <string>
#include <vector>

class ConsoleOutput {
public:
    void drawBoard(
        const Board& board,
        Position cursorPos,
        const MoveCommand& cmd,
        bool hasSelectedPiece,
        Position selectedPos,
        const std::string& message
    );

    void drawResult(bool winner);

private:
    std::string positionToChessNotation(Position pos) const;
    std::string capturedPiecesToString(const std::vector<char>& pieces) const;
};

#endif