#include "Game.h"

Game::Game() : gameState(0), cursorPos(0, 0) {}

void Game::play() {
    while (!checkWin()) {
        output.drawBoard(board, cursorPos);
        cmd = input.getMove();
        handleCommand(cmd);
    }
    output.drawResult(cmd.turn);
}

bool Game::checkWin() const{
    if(cmd.timeup) return true;
    return false;
    // boardを確認してキングがチェックメイトかどうかを判定
}

void Game::handleCommand(MoveCommand cmd) {
    if(cmd.select == 0 && cmd.enter == 0){
        cursorPos = Position(cmd.x, cmd.y);

    }else if(cmd.select == 0 && cmd.enter == 1){
        from = Position(cmd.x, cmd.y);

        if(board.getPiece(from) == nullptr || board.getPiece(from)->getColor() != cmd.turn){
            std::cout << "Invalid Choice!" << std::endl;
        }
        else if(board.getPiece(from) != nullptr){
            cursorPos = Position(cmd.x, cmd.y);
            std::cout << "Successful Choice!" << std::endl;
            std::cout << "You selected " << board.getPiece(from)->getSymbol() << " in " << (char)((int)'a' + cmd.y) << (char)((int)'8' - cmd.x) << std::endl;
            // Hardware側に成功を通知
        }
        // fromの駒が自分のじゃなかったら動かさない

    }else if(cmd.select == 1 && cmd.enter == 0){
        cursorPos = Position(cmd.x, cmd.y);

    }else if(cmd.select == 1 && cmd.enter == 1){
        cursorPos = Position(cmd.x, cmd.y);
        Position to(cmd.x, cmd.y);
        std::cout << to.getRow() << to.getCol() << std::endl;
        if(!board.movePiece(from, to)) std::cout << "Invalid Move!" << std::endl;
        else {
            std::cout << "Successful Move!" << std::endl;
            std::cout << "You moved " << board.getPiece(to)->getSymbol() << " to " << (char)((int)'a' + cmd.y) << (char)((int)'8' - cmd.x) << std::endl;
            // Hardware側に成功を通知
        }
    }
}