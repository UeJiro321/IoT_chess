#include "UI.h"
#include "Core.h"
#include <string>

MoveCommand ConsoleInput::getMove() {
    MoveCommand cmd;
    bool i[10];

    std::cout << "input timeup[9]:turn[8]:select[7]:enter[6]:8-row[5:3]:col[2:0]" << std::endl;
    std::cin >> i[9] >> i[8] >> i[7] >> i[6] >> i[5] >> i[4] >> i[3] >> i[2] >> i[1] >> i[0];

    cmd.timeup = i[9];
    cmd.turn = i[8];
    cmd.select = i[7];
    cmd.enter = i[6];
    cmd.y = i[5]*4 + i[4]*2 + i[3];
    cmd.x = i[2]*4 + i[1]*2 + i[0];

    std::cout << "timeup: " << cmd.timeup << ", turn: " << cmd.turn << ", select: " << cmd.select << ", enter: " << cmd.enter << ", 8-row: " << cmd.y << ", col: " << cmd.x << std::endl;

    return cmd;
}

void ConsoleOutput::drawBoard(const Board& board, Position cursorPos){
    printf("\e[2J");
    printf("\e[2;1H");

    // --- 文字色の定義（エスケープシーケンス） ---
    const char* frameColor = "\033[90m";   // 枠線: 暗いグレー (駒を際立たせるため)
    const char* whiteColor = "\033[1;36m"; // 白駒: 明るいシアン (黒背景で非常に見やすい)
    const char* blackColor = "\033[1;31m"; // 黒駒: 明るい赤 (黒背景で白駒と明確に区別できる)
    const char* resetColor = "\033[0m";    // 色リセット


    for(int i = 0; i < 8; i++){
        std::cout << frameColor << "  +";
        for(int j = 0; j < 8; j++){
            std::cout << "---+";
        }
        std::cout << resetColor << std::endl;

        std::cout << 8-i << " ";
        for(int j = 0; j < 8; j++){
            Position pos(i, j);
            const Piece* p = board.getPiece(pos);
            if(p == nullptr){
                if(pos.getCol() == cursorPos.getCol() && pos.getRow() == cursorPos.getRow()){
                    std::cout << frameColor << "|" << "\033[44m" << "   " << "\033[0m" << resetColor;
                } else {
                    std::cout << frameColor << "|   " << resetColor;
                }
            } else {
                if(p->getColor()){
                    if(pos.getCol() == cursorPos.getCol() && pos.getRow() == cursorPos.getRow()){
                        std::cout << frameColor << "|" << "\033[44m" << whiteColor << " " << p->getSymbol() << " " << resetColor;
                    } else {
                        std::cout << frameColor << "|" << whiteColor << " " << p->getSymbol() << " " << resetColor;
                    }
                } else {
                    if(pos.getCol() == cursorPos.getCol() && pos.getRow() == cursorPos.getRow()){
                        std::cout << frameColor << "|" << "\033[44m" << blackColor << " " << p->getSymbol() << " " << resetColor;
                    } else {
                        std::cout << frameColor << "|" << blackColor << " " << p->getSymbol() << " " << resetColor;
                    }
                }
            }
        }
        
        std::cout << frameColor << "|" << resetColor << std::endl;
    }
    std::cout << frameColor << "  +";
    for(int i = 0; i < 8; i++){
        std::cout << "---+";
    }
    std::cout << resetColor << std::endl;
    std::cout << "   ";
    for(int i = 0; i < 8; i++){
        std::cout << " " << char('a' + i) << " ";
        std::cout << " ";
    }
    std::cout << std::endl;
    std::cout << "Cursor Position: (" << cursorPos.getRow() << ", " << cursorPos.getCol() << ")" << std::endl;
}

void ConsoleOutput::drawResult(bool winner){
    if(winner){
        std::cout << "Red win!" << std::endl;
    } else {
        std::cout << "Blue win!" << std::endl;
    }
}