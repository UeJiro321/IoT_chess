#include "Game.h"
#include <iostream>
#include <unistd.h>

Game::Game()
    : gameState(0),
      cmd{},
      cursorPos(0, 0),
      from(0, 0),
      hasSelectedPiece(false),
      lastMessage("Waiting input"),
      gameOver(false),
      winnerTurn(false)
{
}


namespace {
bool isSameCommand(const MoveCommand& a, const MoveCommand& b)
{
    return a.x      == b.x &&
           a.y      == b.y &&
           a.timeup == b.timeup &&
           a.turn   == b.turn &&
           a.select == b.select &&
           a.enter  == b.enter;
}
}


void Game::play()
{
    output.drawBoard(board, cursorPos, cmd, hasSelectedPiece, from, lastMessage);

    input.gameStart();

    MoveCommand prevCmd{};
    bool hasPrevCmd = false;

    while (!checkWin()) {
        MoveCommand newCmd = input.getMove();

        // 前回と同じ入力なら何もしない
        if (hasPrevCmd && isSameCommand(newCmd, prevCmd)) {
            usleep(10000); // polling interval
            continue;
        }

        // 入力が変化した場合だけ処理する
        cmd = newCmd;
        prevCmd = newCmd;
        hasPrevCmd = true;

        bool isValidCommand = handleCommand(cmd);

        updateGameResult();

        input.notifyDone(isValidCommand);

        usleep(50000); // FPGA側に from_sf を見せる時間

        input.clearDone();

        output.drawBoard(board, cursorPos, cmd, hasSelectedPiece, from, lastMessage);
    }

    output.drawResult(winnerTurn);
}

bool Game::checkWin() const
{
    return gameOver;
}

void Game::updateGameResult()
{
    // 小文字側の king 'k' が取られた場合
    if (!board.isKingAlive(false)) {
        gameOver = true;
        winnerTurn = true;
        lastMessage = "White king was captured. Black wins!";
        return;
    }

    // 大文字側の king 'K' が取られた場合
    if (!board.isKingAlive(true)) {
        gameOver = true;
        winnerTurn = false;
        lastMessage = "Black king was captured. White wins!";
        return;
    }

    if (cmd.timeup) {
        gameOver = true;
        winnerTurn = !cmd.turn;
        lastMessage = "Time up!";
        return;
    }

    gameOver = false;
}

bool Game::handleCommand(MoveCommand cmd)
{
    cursorPos = Position(cmd.x, cmd.y);

    if (cmd.timeup) {
        lastMessage = "Time up";
        return true;
    }

    if (cmd.select == 0 && cmd.enter == 0) {
        lastMessage = "Moving cursor in piece select mode";
        return true;
    }
    else if (cmd.select == 0 && cmd.enter == 1) {
        from = Position(cmd.x, cmd.y);

        const Piece* p = board.getPiece(from);

        if (p == nullptr) {
            hasSelectedPiece = false;
            lastMessage = "No piece on selected square";
            return false;
        }
        else if (p->getColor() != cmd.turn) {
            hasSelectedPiece = false;
            lastMessage = "Selected piece is different color";
            return false;
        }
        else {
            hasSelectedPiece = true;

            lastMessage =
                std::string("Selected ") +
                p->getSymbol() +
                " at " +
                static_cast<char>('a' + cmd.y) +
                static_cast<char>('8' - cmd.x);

            return true;
        }
    }
    else if (cmd.select == 1 && cmd.enter == 0) {
        lastMessage = "Moving cursor in move target select mode";
        return true;
    }
    else if (cmd.select == 1 && cmd.enter == 1) {
        Position to(cmd.x, cmd.y);

        if (!hasSelectedPiece) {
            lastMessage = "No selected piece";
            return false;
        }

        if (!board.movePiece(from, to)) {
            lastMessage = "Invalid move";
            return false;
        }
        else {
            const Piece* movedPiece = board.getPiece(to);

            if (movedPiece != nullptr) {
                lastMessage =
                    std::string("Moved ") +
                    movedPiece->getSymbol() +
                    " to " +
                    static_cast<char>('a' + cmd.y) +
                    static_cast<char>('8' - cmd.x);
            } else {
                lastMessage = "Move success";
            }

            hasSelectedPiece = false;
            return true;
        }
    }
    return false;
}