#include "UI.h"
#include "Core.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

std::string ConsoleOutput::positionToChessNotation(Position pos) const
{
    char file = static_cast<char>('a' + pos.getCol());
    char rank = static_cast<char>('8' - pos.getRow());

    std::string s;
    s += file;
    s += rank;
    return s;
}

std::string ConsoleOutput::capturedPiecesToString(const std::vector<char>& pieces) const
{
    if (pieces.empty()) {
        return "None";
    }

    std::string result;
    for (char c : pieces) {
        result += c;
        result += ' ';
    }

    return result;
}

void ConsoleOutput::drawBoard(
    const Board& board,
    Position cursorPos,
    const MoveCommand& cmd,
    bool hasSelectedPiece,
    Position selectedPos,
    const std::string& message
) {
    printf("\e[2J");
    printf("\e[2;1H");

    // --- 色定義 ---

    const char* frameColor   = "\033[90m";
    const char* whiteColor   = "\033[1;36m";
    const char* blackColor   = "\033[1;31m";

    const char* cursorBlueBg = "\033[44m";  // cmd.turn == 0
    const char* cursorRedBg  = "\033[41m";  // cmd.turn == 1

    const char* headerColor  = "\033[1;37m";
    const char* infoColor    = "\033[1;32m";
    const char* warnColor    = "\033[1;33m";
    const char* errorColor   = "\033[1;31m";
    const char* blinkAttr    = "\033[5m";
    const char* resetColor   = "\033[0m";

    const char* cursorBg = cmd.turn ? cursorBlueBg:cursorRedBg;


    std::string turnText = cmd.turn ? "Blue" : "Red";
    std::string modeText = cmd.select ? "Move Target Select" : "Piece Select";
    std::string inputStateText = cmd.enter ? "Enter" : "Cursor";
    std::string timeupText = cmd.timeup ? "Yes" : "No";

    const Piece* cursorPiece = board.getPiece(cursorPos);
    const Piece* selectedPiece = nullptr;

    if (hasSelectedPiece) {
        selectedPiece = board.getPiece(selectedPos);
    }

    std::cout << headerColor;
    std::cout << "==================================================" << resetColor << std::endl;

    std::cout << infoColor << "Turn        : " << resetColor << turnText << std::endl;
    std::cout << infoColor << "Mode        : " << resetColor << modeText << std::endl;
    std::cout << infoColor << "Input State : " << resetColor << inputStateText << std::endl;

    if (cmd.timeup) {
        std::cout << infoColor << "Time Up     : "
                  << errorColor << timeupText
                  << resetColor << std::endl;
    } else {
        std::cout << infoColor << "Time Up     : "
                  << resetColor << timeupText
                  << std::endl;
    }

    std::cout << std::endl;

    std::cout << infoColor << "Cursor      : " << resetColor
              << positionToChessNotation(cursorPos);

    if (cursorPiece != nullptr) {
        std::cout << "  piece=" << cursorPiece->getSymbol();
    } else {
        std::cout << "  piece=None";
    }

    std::cout << std::endl;

    std::cout << infoColor << "Selected    : " << resetColor;

    if (hasSelectedPiece && selectedPiece != nullptr) {
        std::cout << positionToChessNotation(selectedPos)
                  << selectedPiece->getSymbol();
    } else {
        std::cout << "None";
    }

    std::cout << std::endl;

    std::cout << infoColor << "Captured Blue Pieces : " << resetColor
              << capturedPiecesToString(board.getCapturedBluePieces())
              << std::endl;

    std::cout << infoColor << "Captured Red Pieces : " << resetColor
              << capturedPiecesToString(board.getCapturedRedPieces())
              << std::endl;

    if (!message.empty()) {
        std::cout << infoColor << "Last Result : "
                  << resetColor << message << std::endl;
    } else {
        std::cout << infoColor << "Last Result : "
                  << resetColor << "Waiting input" << std::endl;
    }

    std::cout << headerColor;
    std::cout << "==================================================" << resetColor << std::endl;
    std::cout << std::endl;

    // ==============================
    // Board
    // ==============================
    for (int i = 0; i < 8; i++) {
        std::cout << frameColor << "  +";
        for (int j = 0; j < 8; j++) {
            std::cout << "---+";
        }
        std::cout << resetColor << std::endl;

        std::cout << 8 - i << " ";

        for (int j = 0; j < 8; j++) {
            Position pos(i, j);
            const Piece* p = board.getPiece(pos);

            bool isCursor =
                pos.getRow() == cursorPos.getRow() &&
                pos.getCol() == cursorPos.getCol();

            bool isSelected =
                hasSelectedPiece &&
                pos.getRow() == selectedPos.getRow() &&
                pos.getCol() == selectedPos.getCol();

            std::cout << frameColor << "|" << resetColor;

            if (p == nullptr) {
                if (isCursor) {
                    std::cout << cursorBg << "   " << resetColor;
                } else {
                    std::cout << "   ";
                }
            } else {
                const char* pieceColor = p->getColor() ? whiteColor : blackColor;

                if (isCursor) {
                    std::cout << cursorBg;
                }

                if (isSelected) {
                    std::cout << pieceColor
                              << "[" << p->getSymbol() << "]"
                              << resetColor;
                }
                else{
                    std::cout << pieceColor
                              << " " << p->getSymbol() << " "
                              << resetColor;
                }
            }
        }

        std::cout << frameColor << "|" << resetColor << std::endl;
    }

    std::cout << frameColor << "  +";
    for (int i = 0; i < 8; i++) {
        std::cout << "---+";
    }
    std::cout << resetColor << std::endl;

    std::cout << "   ";
    for (int i = 0; i < 8; i++) {
        std::cout << " " << static_cast<char>('a' + i) << "  ";
    }
    std::cout << std::endl;

    std::cout << std::endl;

    std::cout << cursorBlueBg << "   " << resetColor << " focus (blue turn)  "
              << cursorRedBg  << "   " << resetColor << " focus (red turn)  "
              << std::endl;
}

void ConsoleOutput::drawResult(bool winner)
{
    const char* warnColor    = "\033[1;33m";
    const char* resetColor   = "\033[0m";

    if (winner) {
        std::cout << warnColor << "Blue win!" << resetColor << std::endl;
    } else {
        std::cout << warnColor << "Red win!" << resetColor << std::endl;
    }
}